
AddTest(config_loader_test config_loader_test.cpp)
target_link_libraries(config_loader_test
    config
    )

AddTest(blob_converter_test blob_converter_test.cpp)
target_link_libraries(blob_converter_test
    blob_converter
    )

AddTest(block_to_json_generation block_to_json_generation.cpp)
target_link_libraries(block_to_json_generation
    ametsuchi
    model
    blob_converter
    )