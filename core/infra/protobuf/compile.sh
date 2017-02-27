#! /bin/sh

rm *.cc *.h

protoc  --cpp_out=. *.proto
protoc -I./  --grpc_out=. --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin *.proto
