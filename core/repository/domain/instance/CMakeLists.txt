SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++1y -fPIC")

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

link_directories(
  ${PROJECT_BINARY_DIR}/lib
  ${LEVELDB_PATH}/out-shared
  ${LEVELDB_PATH}/out-static
)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
  ${LEVELDB_PATH}/include
)

ADD_LIBRARY(domain_repository STATIC
  account_repository.cpp
  asset_repository.cpp
)

target_link_libraries(domain_repository
  exception
  # connect infra
  world_state_repo_with_level_db
  event_with_grpc
  protobuf
)
