set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# GenesisBlockClient
add_library(genesis_block_client genesis_block_client.cpp)
target_link_libraries(genesis_block_client
  endpoint
  optional
  model
  )

# BootstrapNetwork
add_library(bootstrap_network bootstrap_network.cpp)
target_link_libraries(bootstrap_network
  genesis_block_client
  rapidjson
  model
  ametsuchi
  ip_tools
  )

# Gflags config validators
add_library(cli-flags_validators validators.cpp)
target_link_libraries(cli-flags_validators gflags)

# IrohaCli
add_executable(iroha-cli
  main.cpp
  validators.cpp
  )
target_link_libraries(iroha-cli
  ed25519
  cli-flags_validators
  bootstrap_network
  rapidjson
  logger
  ametsuchi
  model
  optional
  )
