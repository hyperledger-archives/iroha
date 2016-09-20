
SET(YAML_PATH "${PROJECT_SOURCE_DIR}/core/vendor/yaml-cpp")

set(CROW_PATH "${PROJECT_SOURCE_DIR}/core/vendor/crow")

include_directories(${YAML_PATH}/include)
link_directories(${YAML_PATH}/build)
ADD_LIBRARY(yaml_loader STATIC Yaml.cpp)
target_link_libraries(yaml_loader
  yaml_cpp
)

include_directories(${CROW_PATH}/include)
link_directories(${_PATH}/build)
ADD_LIBRARY(http_server STATIC http_server.cpp)
target_link_libraries(http_server
  yaml-cpp
)

