add_library(processors
    impl/transaction_processor_stub.cpp
    impl/query_processor_stub.cpp
    )

target_link_libraries(processors PUBLIC
    model
    rxcpp
    )
