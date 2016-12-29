set(CMAKE_BUILD_TYPE Debug)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)
set(GTEST_INCLUDE_DIR ${PROJECT_BINARY_DIR}/gtest/src/googletest/googletest/include)

include_directories(
    ${GTEST_INCLUDE_DIR}
)
link_directories(
    ${PROJECT_BINARY_DIR}/lib
)

add_executable(config_test
    config_test.cpp
)
target_link_libraries(config_test
   ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread
)
add_test(
    NAME config_test
    COMMAND $<TARGET_FILE:config_test>
)
