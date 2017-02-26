
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

# Grpc connection Test
add_executable(connection_grpc_test
    connection_with_grpc_test.cpp
)
target_link_libraries(connection_grpc_test
    connection_with_grpc
    consensus_event
    signature
    config_manager
    external_ed25519
    external_gtest
)
add_test(
    NAME connection_grpc_test
    COMMAND $<TARGET_FILE:hash_test>
)








