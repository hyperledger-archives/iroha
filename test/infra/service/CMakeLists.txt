set(CMAKE_BUILD_TYPE Debug)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)
set(JSON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/json")

include_directories(
    ${JSON_PATH}/src

    ${GTEST_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/core
)
link_directories(
    ${PROJECT_BINARY_DIR}/lib
)

add_executable(config_test
    config_test.cpp
)
add_dependencies(config_test googletest)
target_link_libraries(config_test
   ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread
)
add_test(
    NAME config_test
    COMMAND $<TARGET_FILE:config_test>
)



add_executable(http_client_test
    http_client_test.cpp
)
add_dependencies(http_client_test
    googletest
)
target_link_libraries(http_client_test
    ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread
    http_client
    logger

)
add_test(
    NAME http_client_test
    COMMAND $<TARGET_FILE:config_test>
)
