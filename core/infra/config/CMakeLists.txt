set(JSON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/json")

include_directories(
  ${JSON_PATH}/src
)

ADD_LIBRARY(iroha_config_with_json STATIC
    iroha_config_with_json.cpp
)

ADD_LIBRARY(peer_service_with_json STATIC
    peer_service_with_json.cpp
)

TARGET_LINK_LIBRARIES(peer_service_with_json
	logger
)
