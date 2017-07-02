add_library(client_processor
    impl/processor.cpp
    )

target_link_libraries(client_processor PUBLIC
    dao
    rxcpp
    )