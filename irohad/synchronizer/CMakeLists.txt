add_library(synchronizer INTERFACE)

target_include_directories(synchronizer
    INTERFACE interface
    )

target_link_libraries(synchronizer
    INTERFACE rxcpp
    )

add_library(synchronizer_impl
    impl/synchronizer/impl/synchronizer_impl.cpp
    )

target_include_directories(synchronizer_impl
    PUBLIC impl
    )

target_link_libraries(synchronizer_impl
    synchronizer
    ametsuchi
    logger
    )
