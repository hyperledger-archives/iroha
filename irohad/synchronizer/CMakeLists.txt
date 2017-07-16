add_library(synchronizer
    impl/synchronizer.cpp
    )

target_link_libraries(synchronizer PUBLIC
    model
    rxcpp
    )
