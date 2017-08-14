addtest(test_subscriber_testing test_subscriber_testing.cpp)
target_link_libraries(test_subscriber_testing
    rxcpp
    )

add_library(test_block_generator test_block_generator.cpp)
target_link_libraries(test_block_generator
    model
    )

target_include_directories(test_block_generator PUBLIC ${PROJECT_SOURCE_DIR}/test)
