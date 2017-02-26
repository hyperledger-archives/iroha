set(CMAKE_BUILD_TYPE Debug)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

add_executable(config_test
    config_test.cpp
)

target_link_libraries(config_test
   external_gtest
)

add_test(
    NAME config_test
    COMMAND $<TARGET_FILE:config_test>
)
