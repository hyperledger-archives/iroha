SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(
  "${PROJECT_SOURCE_DIR}/core"
)

ADD_LIBRARY(sumeragi STATIC
  sumeragi.cpp
)

target_link_libraries(
  sumeragi
  config_manager
  connection_with_grpc
  signature
  event_repository
  commands
  objects
  external_thread_pool
)
