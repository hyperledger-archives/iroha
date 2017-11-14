addtest(tx_pipeline_integration_test tx_pipeline_integration_test.cpp)
target_link_libraries(tx_pipeline_integration_test
    application
    raw_block_insertion
    model_generators
    )

addtest(transfer_asset_inter_domain_test transfer_asset_inter_domain_test.cpp)
target_link_libraries(transfer_asset_inter_domain_test
  application
  raw_block_insertion
  model_generators
  )
