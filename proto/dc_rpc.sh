#!/bin/bash

cpp_dir="dc_rpc_cpp"

#生成cpp文件
echo "c++ proto build"
if [ ! -d ${cpp_dir} ]; then
    mkdir ${cpp_dir}
else
    echo "${cpp_dir} dir exist"
fi

protoc -I ./ --cpp_out=./${cpp_dir} --grpc_out=./${cpp_dir} --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./dc_rpc.proto
