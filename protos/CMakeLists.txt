execute_process(COMMAND bash "-c" "cd $IROHA_HOME/protos; rm -f $IROHA_HOME/core/infra/protobuf/*.cc $IROHA_HOME/core/infra/protobuf/*.h")
execute_process(COMMAND bash "-c" "cd $IROHA_HOME/protos; protoc  --cpp_out=$IROHA_HOME/core/infra/protobuf *.proto")
execute_process(COMMAND bash "-c" "cd $IROHA_HOME/protos; protoc -I./  --grpc_out=$IROHA_HOME/core/infra/protobuf --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin *.proto")
