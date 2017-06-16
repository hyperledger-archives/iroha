# Temporary comment-outed for compilation
#compile_proto_to_cpp     (api.proto)
#compile_proto_to_grpc_cpp(api_service.proto)

compile_proto_to_cpp(asset.proto)
compile_proto_to_cpp(block.proto)
compile_proto_to_cpp(primitive.proto)
compile_proto_to_cpp(commands.proto)
#compile_proto_to_grpc_cpp(endpoint.proto)

include_directories(${CMAKE_SOURCE_DIR}/schema)
link_directories(${CMAKE_SOURCE_DIR}/schema)
