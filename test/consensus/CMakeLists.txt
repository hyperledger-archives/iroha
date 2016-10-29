SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/my_test_bin)

SET(CMAKE_CXX_FLAGS_DEBUG "-g")

SET(KECCAK_PATH  "${PROJECT_SOURCE_DIR}/core/vendor/KeccakCodePackage")
SET(ED25519_PATH "${PROJECT_SOURCE_DIR}/core/vendor/ed25519")
SET(AERON_PATH  "${PROJECT_SOURCE_DIR}/core/vendor/Aeron")

link_directories(
    ${PROJECT_BINARY_DIR}/lib
    ${ED25519_PATH}
    ${KECCAK_PATH}/bin/generic64
    ${PROJECT_SOURCE_DIR}/core/infra/crypto
    ${AERON_PATH}/cpbuild/Debug/lib
)

add_executable(sumeragi_test_no_gtest
    sumeragi_test.cpp
)
target_link_libraries(sumeragi_test_no_gtest
    snappy
    event_repository
    base64
    signature
    transfer_transaction
    sumeragi
    connection_with_aeron
    peer_service_with_json
    pthread
)
