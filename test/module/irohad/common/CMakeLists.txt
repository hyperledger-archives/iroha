
AddTest(config_loader_test config_loader_test.cpp)
target_link_libraries(config_loader_test
    config
    )

AddTest(blob_converter_test blob_converter_test.cpp)

AddTest(block_insertion_test block_insertion_test.cpp)
target_link_libraries(block_insertion_test
    ametsuchi
    model
    genesis_block_server
    test_block_generator
    logger
    )
