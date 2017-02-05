package daemon

import (
	"fmt"

	docker_image "github.com/docker/docker/image"
)

func (daemon *Daemon) tapconImageBuilt(image *docker_image.Image, tapconData interface{}) {
	sourceinfo, ok := tapconData.(docker_image.Source)
	if !ok {
		fmt.Printf("debug: image without source info, can only be intermediate layers")
		return
	}
	image.Source = sourceinfo
}
