# Temporary comment-outed for compilation
#compile_proto_to_cpp     (api.proto)
#compile_proto_to_grpc_cpp(api_service.proto)

compile_proto_to_cpp(block.proto)
compile_proto_to_cpp(primitive.proto)
compile_proto_to_cpp(commands.proto)
compile_proto_to_grpc_cpp(election.proto)
compile_proto_to_grpc_cpp(endpoint.proto)
compile_proto_to_grpc_cpp(consensus.proto)
compile_proto_to_grpc_cpp(peer_service.proto)

add_dependencies(commandsproto_h primitiveproto_h)
add_dependencies(blockproto_h commandsproto_h)
add_dependencies(endpointproto_h blockproto_h)

add_library(schema
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
    protobuf
    grpc
    grpc++
    grpc++_reflection
)


add_library(consensus_grpc
        consensus.grpc.pb.cc
        consensus.grpc.pb.h
        consensus.pb.cc
        consensus.pb.h
        )
target_link_libraries(consensus_grpc
        protobuf
        grpc++
        )



add_library(peer_service_grpc
        peer_service.grpc.pb.cc
        peer_service.grpc.pb.h
        peer_service.pb.cc
        peer_service.pb.h
        )
target_link_libraries(peer_service_grpc
        protobuf
        grpc++
        )




add_library(election_grpc
        election.pb.h
        election.pb.cc
        election.grpc.pb.h
        election.grpc.pb.cc
        )
target_link_libraries(election_grpc
        protobuf
        grpc++
        )