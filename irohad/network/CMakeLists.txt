add_library(networking
    impl/peer_communication_service_impl.cpp
    )

target_link_libraries(networking PUBLIC
    rxcpp
    model
    ametsuchi
    ordering_service
    synchronizer
    logger
    )
