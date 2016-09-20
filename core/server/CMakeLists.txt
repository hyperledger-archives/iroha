
SET(YAML_PATH "${PROJECT_SOURCE_DIR}/core/vendor/yaml-cpp")
set(CROW_PATH "${PROJECT_SOURCE_DIR}/core/vendor/crow")

include_directories(
  ${YAML_PATH}/include
  ${CROW_PATH}/include
)
link_directories(${YAML_PATH}/build)

ADD_LIBRARY(yaml_loader STATIC Yaml.cpp)
target_link_libraries(yaml_loader
  yaml-cpp
)

include_directories(${CROW_PATH}/include)
ADD_LIBRARY(http_server STATIC http_server.cpp)
target_link_libraries(http_server
  yaml_loader
  boost_system
)

