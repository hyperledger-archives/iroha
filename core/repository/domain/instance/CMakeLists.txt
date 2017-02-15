SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++1y -fPIC")

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

link_directories(${PROJECT_BINARY_DIR}/lib)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
)

ADD_LIBRARY(domain_repository STATIC
  account_repository.cpp
  asset_repository.cpp
)

target_link_libraries(domain_repository
  exception
  # connect infra
  objects
  world_state_repo_with_level_db
  event_with_grpc
  protobuf
  convertor
)
