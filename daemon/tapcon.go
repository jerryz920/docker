package daemon

import (
	"errors"
	"fmt"
	"net"
	"os/exec"
	"sync"

	log "github.com/Sirupsen/logrus"
	"github.com/docker/docker/container"
	docker_image "github.com/docker/docker/image"
)

const MaxLen = 12

func containerChainName(id string) string {
	if len(id) < MaxLen {
		return fmt.Sprintf("ctn-%s", id)
	} else {
		return fmt.Sprintf("ctn-%s", id[:MaxLen])
	}
}

var (
	containerBridgeIp string
	initBridgeFlag    sync.Once
)

func (daemon *Daemon) daemonIp() string {
	bname := daemon.configStore.bridgeConfig.Iface
	log.Info("bridge name ", bname)
	ifaces, err := net.Interfaces()
	if err != nil {
		log.Errorf("error fetching interfaces :%v\n", err)
		return ""
	}
	for _, iface := range ifaces {
		if iface.Name == bname {
			addrs, err := iface.Addrs()
			if err != nil {
				log.Errorf("error fetching addresses :%v\n", err)
				continue
			}
			log.Info("useful addresses: ", addrs)
			for _, addr := range addrs {
				var ip net.IP
				switch v := addr.(type) {
				case *net.IPNet:
					ip = v.IP
				case *net.IPAddr:
					ip = v.IP
				}
				if ip.To4() != nil {
					return ip.String()
				}
			}
		}
	}
	return ""
}

func (daemon *Daemon) tapconImageBuilt(image *docker_image.Image, tapconData interface{}) {
	sourceinfo, ok := tapconData.(docker_image.Source)
	if !ok {
		fmt.Printf("debug: image without source info, can only be intermediate layers")
		return
	}
	image.Source = sourceinfo
}

func (daemon *Daemon) tapconSetupFirewall(container *container.Container) error {
	log.Info("TapconDebug: start firewall")
	initBridgeFlag.Do(func() {
		containerBridgeIp = daemon.daemonIp()
		log.Infof("TapconDebug: obtain container bridge IP: [%s]", containerBridgeIp)
	})

	id := container.ID
	chainName := containerChainName(id)
	cmd := exec.Command("iptables", "-t", "nat", "-N", chainName)

	if out, err := cmd.CombinedOutput(); err != nil {
		log.Errorf("error creating chain: %s", string(out))
		return err
	}

	cmd = exec.Command("iptables", "-t", "nat", "-I", "POSTROUTING", "-j", chainName)
	if out, err := cmd.CombinedOutput(); err != nil {
		log.Errorf("error inserting chain: %s", string(out))
		// we ignore the error here...
		exec.Command("iptables", "-t", "nat", "-X", chainName).Run()
		return err
	}
	var containerIp string
	if n, ok := container.NetworkSettings.Networks["bridge"]; ok {
		containerIp = n.IPAddress
	} else {
		log.Debug("container network setting: %v", container.NetworkSettings.Networks)
		exec.Command("iptables", "-t", "nat", "-X", chainName).Run()
		return errors.New("only support bridge network")
	}
	// TODO: should add a drop rule to range besides [port_min, port_max]
	// only support Tcp for now
	cmd = exec.Command("iptables", "-t", "nat", "-A", chainName,
		"-p", "tcp", "-s", containerIp,
		"-j", "SNAT", "--to-source",
		fmt.Sprintf("%s", containerBridgeIp))
	if out, err := cmd.CombinedOutput(); err != nil {
		log.Errorf("error inserting static mapping rule: %s", string(out))
		// clear the chain
		exec.Command("iptables", "-t", "nat", "-F", chainName).Run()
		return err
	}

	return nil
}

func (daemon *Daemon) tapconRemoveFirewall(container *container.Container) error {

	log.Info("TapconDebug: removing firewall")
	id := container.ID
	chainName := containerChainName(id)
	cmd := exec.Command("iptables", "-t", "nat", "-D", "POSTROUTING", "-j", chainName)
	if out, err := cmd.CombinedOutput(); err != nil {
		log.Errorf("error clearing jumping to static mapping chain: %s", string(out))
		return err
	}

	cmd = exec.Command("iptables", "-t", "nat", "-F", chainName)
	if out, err := cmd.CombinedOutput(); err != nil {
		log.Errorf("error clearing static mapping chain: %s", string(out))
		return err
	}

	cmd = exec.Command("iptables", "-t", "nat", "-X", chainName)
	if out, err := cmd.CombinedOutput(); err != nil {
		log.Errorf("error deleting static mapping chain: %s", string(out))
		return err
	}
	return nil
}
