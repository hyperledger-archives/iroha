set(CMAKE_BUILD_TYPE Debug)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

include_directories(
    ${GTEST_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/core
)
link_directories(
    ${PROJECT_BINARY_DIR}/lib
)

add_executable(java_data_structure_test
    java_data_structure_test.cpp
)
add_dependencies(java_data_structure_test googletest)
target_link_libraries(java_data_structure_test
   ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread
)
add_test(
    NAME java_data_structure_test
    COMMAND $<TARGET_FILE:java_data_structure_test>
)
