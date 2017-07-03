add_library(client_processor
    impl/processor.cpp
    impl/transaction_processor_stub.cpp
    impl/stub_query_processor.cpp
    impl/client_processor_stub.cpp
    )

target_link_libraries(client_processor PUBLIC
    dao
    rxcpp
    )