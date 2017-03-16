SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(iroha-main
  main.cpp
)
target_link_libraries(iroha-main
  connection_with_grpc
  sumeragi
  config_manager
  leveldb
  http_server_with_cappuccino
  izanami
  logger
)
