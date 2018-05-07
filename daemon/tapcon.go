package daemon

import (
	"fmt"
	"net"
	"os/exec"
	"sync"
	"unsafe"

	log "github.com/Sirupsen/logrus"
	"github.com/docker/docker/container"
	docker_image "github.com/docker/docker/image"
)

/*

#include <stdlib.h>
#include "libport.h"
#cgo LDFLAGS: -llatte

*/
import "C"

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
	/// We do not need this SNAT setup now as we have a full new IP address
	//var containerIp string
	//if n, ok := container.NetworkSettings.Networks["bridge"]; ok {
	//	containerIp = n.IPAddress
	//} else {
	//	log.Debug("container network setting: %v", container.NetworkSettings.Networks)
	//	exec.Command("iptables", "-t", "nat", "-X", chainName).Run()
	//	return errors.New("only support bridge network")
	//}
	//// TODO: should add a drop rule to range besides [port_min, port_max]
	//// only support Tcp for now
	//cmd = exec.Command("iptables", "-t", "nat", "-A", chainName,
	//	"-p", "tcp", "-s", containerIp,
	//	"-j", "SNAT", "--to-source",
	//	fmt.Sprintf("%s", containerBridgeIp))
	//if out, err := cmd.CombinedOutput(); err != nil {
	//	log.Errorf("error inserting static mapping rule: %s", string(out))
	//	// clear the chain
	//	exec.Command("iptables", "-t", "nat", "-F", chainName).Run()
	//	return err
	//}

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

func (daemon *Daemon) tapconStartContainer(container *container.Container) error {

	cimage := C.CString(fmt.Sprintf("%s#%s", container.ImageID.String(), "master"))
	cpid := C.uint64_t(container.State.Pid)
	cnport := C.int(0)
	cstore := C.CString("") /// use itself
	configs := make([]string, 0)

	for _, env := range container.Config.Env {
		configs = append(configs, env)
	}

	index := 0
	for _, part := range container.Config.Entrypoint {
		conf := fmt.Sprintf("arg%d=%s", index, part)
		configs = append(configs, conf)
		index++
	}

	for _, part := range container.Config.Cmd {
		conf := fmt.Sprintf("arg%d=%s", index, part)
		configs = append(configs, conf)
		index++
	}
	/// A lot of copy, sigh.
	cconfigs := make([]*C.char, len(configs))

	for i := 0; i < len(configs); i++ {
		cconfigs[i] = C.CString(configs[i])
	}
	cn := C.int(len(configs))
	cbufptr := (**C.char)(unsafe.Pointer(&cconfigs[0]))

	if n, ok := container.NetworkSettings.Networks["bridge"]; ok {
		containerIp := n.IPAddress
		log.Info("creating container on IP: ", containerIp)
		cip := C.CString(containerIp)
		pmin, _ := C.liblatte_create_instance(
			cpid, cimage, cip, cnport, cstore, cbufptr, cn)
		if pmin <= 0 {
			log.Info("fail to create principal")
		}
		C.free(unsafe.Pointer(cip))
		container.TapconInstanceID = (uint64)(container.State.Pid)

	} else {
		log.Info("fail to obtain network address of container")
		container.TapconInstanceID = 0
	}
	for i := 0; i < len(configs); i++ {
		C.free(unsafe.Pointer(cconfigs[i]))
	}
	C.free(unsafe.Pointer(cstore))
	C.free(unsafe.Pointer(cimage))
	return nil
}

func (daemon *Daemon) tapconStopContainer(container *container.Container) error {
	cpid := C.uint64_t(container.TapconInstanceID)
	ret, _ := C.liblatte_delete_instance(cpid)
	if ret < 0 {
		log.Error("fail to delete the instance")
	}
	return nil
}

func (daemon *Daemon) TapconEndorseImage(image string, source string) error {
	cimageID := C.CString(image)
	/// git # master : dir
	csource := C.CString(source)
	//url := b.sourceCtx.GitURL() + "#" + string(b.sourceCtx.IdentityHash()) + ":" +
	//	hex.EncodeToString(b.sourceCtx.CwdHash())
	cprop := C.CString("source")
	ret, _ := C.liblatte_endorse(cimageID, cprop, csource)
	if ret != 0 {
		log.Errorf("error creating image %s in metadata service", image)
	}
	chost := C.CString("")
	ret, _ = C.liblatte_link_image(chost, cimageID)
	if ret != 0 {
		log.Errorf("error linking image %s to self", image)
	}
	C.free(unsafe.Pointer(cimageID))
	C.free(unsafe.Pointer(csource))
	C.free(unsafe.Pointer(cprop))
	C.free(unsafe.Pointer(chost))
	return nil
}
