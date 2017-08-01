addtest(bootstrap_network_test bootstrap_network_test.cpp)
target_link_libraries(bootstrap_network_test
    bootstrap_network
    crypto
    ametsuchi
    )

addtest(client_test client_test.cpp)
target_link_libraries(client_test
    client
    processors
    server_runner
    )
