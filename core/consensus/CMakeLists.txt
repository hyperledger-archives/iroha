SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(
  "${PROJECT_SOURCE_DIR}/core"
  "${PROJECT_SOURCE_DIR}/core/vendor/thread_pool_cpp/thread_pool"
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
)

ADD_LIBRARY(consensus_event STATIC
  consensus_event.cpp
)

target_link_libraries(consensus_event
  config_manager
  commands
  objects
  validator
  hash
)
