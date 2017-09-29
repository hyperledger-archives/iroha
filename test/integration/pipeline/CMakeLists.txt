addtest(tx_pipeline_integration_test tx_pipeline_integration_test.cpp)
target_link_libraries(tx_pipeline_integration_test
    application
    raw_block_insertion
    model_generators
    )
