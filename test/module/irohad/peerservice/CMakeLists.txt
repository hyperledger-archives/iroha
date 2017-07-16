AddTest(peerservice_test peerservice_test.cpp)
target_link_libraries(peerservice_test
        peer_service
        grpc++
        peer_service_grpc
        crypto
        )