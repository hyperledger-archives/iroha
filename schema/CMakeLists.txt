# Temporary comment-outed for compilation
#compile_proto_to_cpp     (api.proto)
#compile_proto_to_grpc_cpp(api_service.proto)

compile_proto_to_cpp(asset.proto)
compile_proto_to_cpp(block.proto)
compile_proto_to_cpp(primitive.proto)
compile_proto_to_cpp(commands.proto)
#compile_proto_to_grpc_cpp(endpoint.proto)

add_library(schema
    asset.pb.cc
    block.pb.cc
    commands.pb.cc
    primitive.pb.cc
)
target_link_libraries(schema
    protobuf
)

add_library(endpoint
    endpoint.pb.cc
    endpoint.grpc.pb.cc
)
target_link_libraries(endpoint
    grpc++
    grpc
    gpr
    pthread
    dl
)
