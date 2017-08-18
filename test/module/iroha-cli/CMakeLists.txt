addtest(bootstrap_network_test bootstrap_network_test.cpp)
target_link_libraries(bootstrap_network_test
    bootstrap_network
    crypto
    )
target_include_directories(bootstrap_network_test PUBLIC
    ${PROJECT_SOURCE_DIR}/iroha-cli
    )
