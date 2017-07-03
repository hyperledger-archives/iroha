add_library(torii
    impl/processor.cpp
    impl/stub_query_processor.cpp
    impl/transaction_processor_stub.cpp
    )

target_link_libraries(torii PUBLIC
    dao
    rxcpp
    )