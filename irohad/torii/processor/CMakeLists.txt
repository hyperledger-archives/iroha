add_library(torii
    impl/processor.cpp
    impl/stub_query_processor.cpp
    )

target_link_libraries(torii PUBLIC
    dao
    rxcpp
    )