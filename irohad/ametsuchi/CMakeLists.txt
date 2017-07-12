add_library(ametsuchi
    impl/storage_impl.cpp
    )

target_link_libraries(ametsuchi
    rxcpp
    optional
    )