set(JSON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/json")

include_directories(
  ${JSON_PATH}/src
  ${PROJECT_SOURCE_DIR}/core
)

ADD_LIBRARY(http_server_with_cappuccino STATIC 
  http_server_with_cappuccino.cpp
)

target_link_libraries(http_server_with_cappuccino
  ssl
  crypto
  logger
  datetime
  objects
  base64
  signature
  convertor
  connection_with_grpc
  event_repository
)

