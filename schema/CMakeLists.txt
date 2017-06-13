
# ToDo use macro
#execute_process(COMMAND "protoc -I. --cpp_out=. --proto_path=commands.proto block.proto")
#execute_process(COMMAND "protoc -I. --cpp_out=. --proto_path=commands.proto asset.proto")
#execute_process(COMMAND "protoc -I. --cpp_out=. --proto_path=commands.proto commands.proto")
#execute_process(COMMAND "protoc -I. --cpp_out=. --proto_path=commands.proto primitive.proto")

ADD_LIBRARY(schema STATIC
    asset.pb.cc
    block.pb.cc
    commands.pb.cc
    primitive.pb.cc
)
target_link_libraries(schema
    protobuf
)

ADD_LIBRARY(endpoint STATIC
    endpoint.pb.cc
    endpoint.grpc.pb.cc
)
target_link_libraries(endpoint
    grpc++
    grpc++_reflection
    pthread
    dl
)
