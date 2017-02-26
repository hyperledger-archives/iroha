set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

set(CMAKE_BUILD_TYPE Debug)

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

link_directories(
    ${LEVELDB_PATH}/out-static
)

add_executable(transaction_builder_test
    transaction_builder_add_test.cpp
    transaction_builder_transfer_test.cpp
    transaction_builder_update_test.cpp
    transaction_builder_remove_test.cpp
    transaction_builder_unset_test.cpp
)

target_link_libraries(transaction_builder_test
    event_with_grpc
    exception
    external_gtest
)

add_test(
    NAME transaction_builder_test
    COMMAND $<TARGET_FILE:transaction_builder_test>
)
