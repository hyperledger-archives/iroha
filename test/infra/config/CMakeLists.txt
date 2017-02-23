
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

SET(CMAKE_BUILD_TYPE Debug)
SET(GTEST_INCLUDE_DIR ${PROJECT_BINARY_DIR}/gtest/src/googletest/googletest/include)

include_directories(
    ${GTEST_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/core
    ${PROJECT_SOURCE_DIR}/core/vendor/json/src
)
link_directories(
    ${PROJECT_BINARY_DIR}/lib
)

add_executable(ensure_sumeragi_json_format_test
    ensure_sumeragi_json_format.cpp)
target_link_libraries(ensure_sumeragi_json_format_test
    config_manager
    config_format
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    pthread
)

add_test(
    NAME ensure_sumeragi_json_format_test
    COMMAND $<TARGET_FILE:ensure_sumeragi_json_format_test>
)

add_executable(peer_service_with_json_test
    peer_service_with_json_test.cpp)
target_link_libraries(peer_service_with_json_test
    config_manager
    config_format
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    pthread
)
add_test(
    NAME peer_service_with_json_test
    COMMAND $<TARGET_FILE:peer_service_with_json_test>
)
