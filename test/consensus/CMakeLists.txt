SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/my_test_bin)

SET(CMAKE_CXX_FLAGS_DEBUG "-g")

SET(KECCAK_PATH  "${PROJECT_SOURCE_DIR}/core/vendor/KeccakCodePackage")
SET(ED25519_PATH "${PROJECT_SOURCE_DIR}/core/vendor/ed25519")
SET(LEVELDB_PATH "${PROJECT_SOURCE_DIR}/core/vendor/leveldb")
SET(CRYPTO_PATH  "${PROJECT_SOURCE_DIR}/core/infra/crypto")

include_directories(
    ${PROJECT_SOURCE_DIR}/core
    ${PROJECT_SOURCE_DIR}/core/vendor/json/src
)

link_directories(
    ${PROJECT_BINARY_DIR}/lib
    ${ED25519_PATH}/lib
    ${KECCAK_PATH}/bin/generic64
    ${PROJECT_SOURCE_DIR}/core/infra/crypto
    ${AERON_PATH}/cppbuild/Release/lib
    ${LEVELDB_PATH}/out-static

    ${CRYPTO_PATH}
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
    pthread
    consensus_event
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
