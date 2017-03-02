include_directories(
  ${PROJECT_SOURCE_DIR}/core
)

add_library(config_manager STATIC
  peer_service_with_json.cpp
  iroha_config_with_json.cpp
)

target_link_libraries(config_manager
	config_format
	exception
	logger
	json
)

add_library(config_format STATIC
	config_format.cpp
)
