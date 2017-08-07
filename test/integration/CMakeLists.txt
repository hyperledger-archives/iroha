set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

add_subdirectory(consensus)

addtest(client_test client_test.cpp)
target_link_libraries(client_test
    client
    processors
    server_runner
    )
target_include_directories(client_test PUBLIC
    ${PROJECT_SOURCE_DIR}/iroha-cli
    )

add_subdirectory(pipeline)
