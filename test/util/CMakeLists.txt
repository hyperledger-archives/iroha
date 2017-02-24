set(CMAKE_BUILD_TYPE Debug)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

include_directories(
    ${GTEST_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/core
)
link_directories(
    ${PROJECT_BINARY_DIR}/lib
)

add_executable(stringify_vector_test
    stringify_vector_test.cpp
)
add_dependencies(stringify_vector_test googletest)
target_link_libraries(stringify_vector_test
   ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread
)
add_test(
    NAME stringify_vector_test
    COMMAND $<TARGET_FILE:stringify_vector_test>
)
