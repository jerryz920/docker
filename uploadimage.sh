#!/bin/bash

imageid=$1
if [ $# -lt 1 ]; then
  echo Must provide image ID
  return
fi

echo "Image to upload: $imageid"

jsonfiy() {
  local name=$1
  echo "{" > $name.json
  echo " \"image\": \"$imageid\"," >> $name.json
  echo " \"files\": [" >> $name.json
  for line in `head -n -1 $name`; do
    echo "\"$line\"," >> $name.json
  done
  echo "\"`tail -n 1 $name`\"" >> $name.json
  echo "]" >> $name.json
  echo "}" >> $name.json
}

pushd .
mkdir -p /tmp/docker
cd /tmp/docker
rm -rf *
docker image save $imageid -o image.tar
tar xf image.tar
layer=`find . -name layer.tar`
tar xf $layer
rm -f $layer
find . -type f -exec sha1sum {} \; | awk '{print $1}' | sort | uniq > $imageid
popd
cp /tmp/docker/$imageid .
jsonfiy $imageid


# curl http://10.10.1.39:19851/upload_image -d "


