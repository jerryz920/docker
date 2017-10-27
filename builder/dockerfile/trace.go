package dockerfile

import (
	"fmt"
	"os"
	"sync"
	"time"

	"github.com/Sirupsen/logrus"
)

const (
	TraceFilePrefix   = "btrace"
	TraceDir          = "/var/run/docker-btrace/"
	ContainerTraceDir = "/trace"
)

var (
	initFlag sync.Once
)

func createTraceDir() {
	info, err := os.Stat(TraceDir)
	if err != nil && !os.IsNotExist(err) {
		logrus.Errorf("Error to stat trace dir %s: %v\n", TraceDir, err)
		return
	}
	if os.IsNotExist(err) {
		err = os.Mkdir(TraceDir, 0755)
		if err != nil {
			logrus.Errorf("Error to create trace dir %s: %v\n", TraceDir, err)
			return
		}
	} else {
		if !info.IsDir() {
			logrus.Errorf("trace dir exist as non-dir %s\n", TraceDir)
		}
	}

}

func (b *Builder) TraceInit(name string) {
	initFlag.Do(createTraceDir)
	logrus.Debug("Tapcon start image build trace")

	ts := time.Now().Unix()
	b.traceKey = fmt.Sprintf("%d", ts)
	b.traceName = name
	dirName := fmt.Sprintf("%s/%s", ContainerTraceDir, name)
	if _, err := os.Stat(dirName); err != nil {
		if os.IsNotExist(err) {
			os.Mkdir(dirName, 0755)
		}
	}
}

func (b *Builder) TraceCmdPrefix(id int) []string {
	traceFileName := fmt.Sprintf("%s/%s/log-%s.%d", ContainerTraceDir, b.traceName, b.traceKey, id)
	return []string{"strace", "-e", "trace=open,process,network", "-ff", "-o", traceFileName,
		"sh", "-c"}
}

func (b *Builder) setupTraceVolume() []string {
	return []string{fmt.Sprintf("%s:%s:rw", TraceDir, ContainerTraceDir)}
}
