addtest(bootstrap_network_test bootstrap_network_test.cpp)
target_link_libraries(bootstrap_network_test
    bootstrap_network
    crypto
    ametsuchi
    )
target_include_directories(bootstrap_network_test PUBLIC
    ${PROJECT_SOURCE_DIR}/iroha-cli
    )

addtest(client_test client_test.cpp)
target_link_libraries(client_test
    client
    processors
    server_runner
    )
target_include_directories(client_test PUBLIC
    ${PROJECT_SOURCE_DIR}/iroha-cli
    )
