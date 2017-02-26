
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)


SET(CRYPTO_PATH  "${PROJECT_SOURCE_DIR}/core/infra/crypto")
SET(ED25519_PATH "${PROJECT_SOURCE_DIR}/core/vendor/ed25519")

include_directories(
    ${GTEST_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/core
)

link_directories(
    ${PROJECT_BINARY_DIR}/lib
)

link_directories(
    ${CRYPTO_PATH}
    ${ED25519_PATH}/lib
)

# Grpc connection Test
add_executable(connection_grpc_test
    connection_with_grpc_test.cpp
)
add_dependencies(connection_grpc_test googletest)
target_link_libraries(connection_grpc_test
    connection_with_grpc
    consensus_event
    signature
    config_manager
    ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread
)
add_test(
    NAME connection_grpc_test
    COMMAND $<TARGET_FILE:hash_test>
)








