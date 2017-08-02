
AddTest(config_loader_test config_loader_test.cpp)
target_link_libraries(config_loader_test
    config
    )

AddTest(blob_converter_test blob_converter_test.cpp)
target_link_libraries(blob_converter_test
    blob_converter
    )

AddTest(block_inserion_test block_inserion_test.cpp)
target_link_libraries(block_inserion_test
    ametsuchi
    model
    blob_converter
    genesis_block_server
    )