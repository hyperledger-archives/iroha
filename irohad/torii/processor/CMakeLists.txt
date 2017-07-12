add_library(client_processor
    impl/transaction_processor_stub.cpp
    impl/query_processor_stub.cpp
    )

target_link_libraries(client_processor PUBLIC
    model
    rxcpp
    )
