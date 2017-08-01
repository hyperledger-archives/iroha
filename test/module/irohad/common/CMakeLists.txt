
AddTest(config_loader_test config_loader_test.cpp)
target_link_libraries(config_loader_test
    config
    )

AddTest(blob_converter_test blob_converter_test.cpp)
target_link_libraries(blob_converter_test
    blob_converter
    )