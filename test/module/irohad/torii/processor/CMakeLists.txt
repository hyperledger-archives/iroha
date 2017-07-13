# Testing of transaction processor
addtest(transaction_processor_test transaction_processor_test.cpp)
target_link_libraries(transaction_processor_test
    processors
)