add_library(networking
    impl/peer_communication_service.cpp
    peer_communication_stub.cpp
    )

target_link_libraries(networking PUBLIC
    rxcpp
    model
    ametsuchi
    )
