addtest(bootstrap_network_test bootstrap_network_test.cpp)
target_link_libraries(bootstrap_network_test
  bootstrap_network
  crypto
  ametsuchi
  )

