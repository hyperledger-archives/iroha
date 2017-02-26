
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)


SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++1y")

SET(AERON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/Aeron")
SET(LEVELDB_PATH "${PROJECT_SOURCE_DIR}/core/vendor/leveldb")
SET(JSON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/json")

include_directories(
  ${PROJECT_SOURCE_DIR}/core
  ${PROJECT_SOURCE_DIR}/core/server
  ${PROJECT_SOURCE_DIR}/core/peer
  ${LEVELDB_PATH}/include
  ${AERON_PATH}/aeron-client/src/main/cpp
  ${JSON_PATH}/src
)

link_directories(
  ${PROJECT_BINARY_DIR}/lib
  ${AERON_PATH}/cppbuild/Release/lib
  ${LEVELDB_PATH}/out-static
)

add_executable(iroha-main
  main.cpp
)
target_link_libraries(iroha-main
  # used grpc
  grpc++_reflection
  protobuf
  pthread
  dl

  leveldb
  snappy
  world_state_repo_with_level_db
  sumeragi
  logger
  http_server_with_cappuccino
  connection_with_grpc
  config_manager
  pthread
  external_keccak
  hash
  external_thread_pool
)
