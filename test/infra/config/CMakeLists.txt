SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

SET(CMAKE_BUILD_TYPE Debug)


SET(CRYPTO_PATH  "${PROJECT_SOURCE_DIR}/core/infra/crypto")
SET(ED25519_PATH "${PROJECT_SOURCE_DIR}/core/vendor/ed25519")

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

add_executable(ensure_sumeragi_json_format_test
    ensure_sumeragi_json_format.cpp
)

target_link_libraries(ensure_sumeragi_json_format_test
    config_manager
    json
    gtest
)

add_test(
    NAME ensure_sumeragi_json_format_test
    COMMAND $<TARGET_FILE:ensure_sumeragi_json_format_test>
)

add_executable(peer_service_with_json_test
    peer_service_with_json_test.cpp)
target_link_libraries(peer_service_with_json_test
    config_manager
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    pthread
    connection_with_grpc
    consensus_event
    signature
    config_manager
    event_with_grpc
)
add_test(
    NAME peer_service_with_json_test
    COMMAND $<TARGET_FILE:peer_service_with_json_test>
)
