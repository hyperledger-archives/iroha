
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SET(YAML_PATH  "${PROJECT_SOURCE_DIR}/core/vendor/yaml-cpp")
SET(AERON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/Aeron")
SET(MESSAGE_PACK_PATH ${PROJECT_SOURCE_DIR}/core/vendor/msgpack-c)
SET(LEVELDB_PATH ${PROJECT_SOURCE_DIR}/core/vendor/leveldb)
SET(KECCAK_PATH  "${PROJECT_SOURCE_DIR}/core/vendor/KeccakCodePackage")
SET(ED25519_PATH "${PROJECT_SOURCE_DIR}/core/vendor/ed25519")

include_directories(
  ${PROJECT_SOURCE_DIR}/core/server
  ${PROJECT_SOURCE_DIR}/core/peer

  ${YAML_PATH}/include
  ${LEVELDB_PATH}/include
  ${AERON_PATH}/aeron-client/src/main/cpp
  ${MESSAGE_PACK_PATH}/include
  ${ED25519_PATH}/src
)

link_directories(
  ${PROJECT_BINARY_DIR}/lib
  ${ED25519_PATH}
  ${KECCAK_PATH}/bin/generic64
  ${PROJECT_SOURCE_DIR}/core/infra/crypto 
  ${YAML_PATH}/build
  ${AERON_PATH}/cppbuild/Debug/lib
  ${MESSAGE_PACK_PATH}/
  ${LEVELDB_PATH}/out-static
)

add_executable(iroha-main
  main.cpp
)
target_link_libraries(iroha-main
  event_repository
  merkle_transaction_repository
  world_state_repo_with_level_db
  unconfirmed_transaction_repository
  sumeragi
  snappy
  logger
  yaml_loader
  http_server_with_crow
  connection_with_aeron
  peer_service
  pthread
)

