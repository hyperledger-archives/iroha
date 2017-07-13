# Testing of transaction processor
add_executable(transaction_processor_test transaction_processor_test.cpp)

target_link_libraries(
    transaction_processor_test
    processors
    gtest
)

add_test(
    NAME transaction_processor_test
    COMMAND $<TARGET_FILE:transaction_processor_test>
)
