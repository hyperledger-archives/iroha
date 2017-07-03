add_library(networking
    impl/network_api.cpp
    )

target_link_libraries(networking PUBLIC
    rxcpp
    model
    ametsuchi
    )
