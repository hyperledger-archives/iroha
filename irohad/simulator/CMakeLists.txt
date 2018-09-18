add_library(simulator INTERFACE)

target_include_directories(simulator
    INTERFACE interface
    )

target_link_libraries(simulator INTERFACE
    shared_model_interfaces
    rxcpp
    )

add_library(simulator_impl
    impl/simulator/impl/simulator.cpp)

target_include_directories(simulator_impl
        PUBLIC impl
    )

target_link_libraries(simulator_impl
    simulator
    logger
    )
