add_library(config_manager STATIC
  peer_service_with_json.cpp
  iroha_config_with_json.cpp
  config_format.cpp
  config_utils.cpp
)

target_link_libraries(config_manager
  json
  expected
  logger
  ip_tools
)
