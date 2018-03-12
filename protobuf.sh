#!/bin/bash
go get -u google.golang.org/grpc
go get -u github.com/golang/protobuf/protoc-gen-go
go get -u github.com/google/protobuf
cd $GOPATH/src/github.com/google/protobuf/
# may want some better way?
#git checkout -b dev $1
./autogen.sh
./configure
make -j 4
make install

