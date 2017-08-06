# TODO: this does not work reliably
#AddTest(peerservice_test peerservice_test.cpp)
#target_link_libraries(peerservice_test
#        peer_service
#        grpc++
#        peer_service_grpc
#        crypto
#        )


add_executable(peerservice_peer
        peerservice_peer.cpp
        )
target_link_libraries(peerservice_peer
        peer_service
        uvw
        )
