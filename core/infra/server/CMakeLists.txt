
SET(YAML_PATH "${PROJECT_SOURCE_DIR}/core/vendor/yaml-cpp")
set(CROW_PATH "${PROJECT_SOURCE_DIR}/core/vendor/crow")
set(JSON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/json")

include_directories(
  ${CROW_PATH}/include
  ${JSON_PATH}/src
)

include_directories(${CROW_PATH}/include)
ADD_LIBRARY(http_server_with_crow STATIC http_server_with_crow.cpp)
target_link_libraries(http_server_with_crow
  boost_system
  ssl
  crypto
)