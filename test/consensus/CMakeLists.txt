SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/my_test_bin)

SET(CMAKE_CXX_FLAGS_DEBUG "-g")

SET(LEVELDB_PATH "${PROJECT_SOURCE_DIR}/core/vendor/leveldb")
SET(CRYPTO_PATH  "${PROJECT_SOURCE_DIR}/core/infra/crypto")

include_directories(
    ${PROJECT_SOURCE_DIR}/core
    ${PROJECT_SOURCE_DIR}/core/vendor/json/src
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
    pthread
    consensus_event
    config_format
)
