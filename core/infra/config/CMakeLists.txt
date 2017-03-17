###############################################################
# If peer service config migrates to peer service completely,
# following linking is no more needed.

add_library(peer_service_config STATIC
  peer_service_with_json.cpp
)

target_link_libraries(peer_service_config
  http_server_with_cappuccino
)
###############################################################

add_library(config_manager STATIC
  # peer_service_with_json.cpp # eliminate tempolaly
  iroha_config_with_json.cpp
  config_format.cpp
)

target_link_libraries(config_manager
  json
  exception
  logger

  peer_service_config # add tempolary
)
