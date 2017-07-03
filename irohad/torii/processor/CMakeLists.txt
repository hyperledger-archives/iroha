add_library(torii
    impl/processor.cpp
    )

target_link_libraries(torii PUBLIC
    model
    rxcpp
    )
