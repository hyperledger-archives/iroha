include_directories(
  ${PROJECT_SOURCE_DIR}/core/vendor/json/src
  ${PROJECT_SOURCE_DIR}/core
)

add_library(config_manager STATIC
  peer_service_with_json.cpp
  iroha_config_with_json.cpp
)
