addtest(stateless_transaction_validator_test stateless/transaction_validator_test.cpp)
target_link_libraries(stateless_transaction_validator_test
    stateless_validator
    hash
    crypto
    schema
    model
    )

addtest(query_execution query_execution.cpp)
target_link_libraries(query_execution
    model
    )

addtest(chain_validation_test chain_validation_test.cpp)
target_link_libraries(chain_validation_test
    chain_validator
    )
