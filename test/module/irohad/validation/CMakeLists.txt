set(CMAKE_BUILD_TYPE Debug)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

addtest(stateless_transaction_validator_test stateless/transaction_validator_test.cpp)
target_link_libraries(stateless_transaction_validator_test
        stateless_validator
        hash
        crypto
        schema
        model
        )

addtest(stateful_validation_test stateful_validation_test.cpp)
target_link_libraries(stateful_validation_test
        model
        )

addtest(query_execution query_execution.cpp)
target_link_libraries(query_execution
        model
        )


