set(CMAKE_BUILD_TYPE Debug)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

addtest(block_serializer_test block_serializer_test.cpp)
target_link_libraries(block_serializer_test
    model
    ametsuchi
    )

addtest(ametsuchi_test ametsuchi_test.cpp)
target_link_libraries(ametsuchi_test
    ametsuchi
    model
    )

addtest(flat_file_test flat_file_test.cpp)
target_link_libraries(flat_file_test
    ametsuchi
    )
