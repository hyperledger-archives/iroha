SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/my_test_bin)

SET(CMAKE_CXX_FLAGS_DEBUG "-g")

SET(LEVELDB_PATH "${PROJECT_SOURCE_DIR}/core/vendor/leveldb")

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

link_directories(
    ${AERON_PATH}/cppbuild/Release/lib
    ${LEVELDB_PATH}/out-static
)

add_executable(sumeragi_test
    sumeragi_test.cpp
)


target_link_libraries(sumeragi_test
    leveldb
    snappy
    base64
    signature
    sumeragi
    connection_with_grpc
    config_manager
)

add_executable(transaction_test
    transaction_test.cpp
)
target_link_libraries(transaction_test
    snappy
    base64
    signature
    sumeragi
    connection_with_grpc
    config_manager
)
