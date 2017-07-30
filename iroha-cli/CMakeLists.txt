set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_library(cli_client
    client.cpp
    )

target_link_libraries(cli_client
    endpoint ametsuchi model)

add_executable(cli-main main.cpp validators.cpp)
target_link_libraries(cli-main gflags ed25519 cli_client)
