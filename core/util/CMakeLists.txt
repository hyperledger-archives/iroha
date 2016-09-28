SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(YAML_PATH "${PROJECT_SOURCE_DIR}/core/vendor/yaml-cpp")

# Datetime
ADD_LIBRARY(datetime  STATIC datetime.cpp)

# Logger
ADD_LIBRARY(logger  STATIC logger.cpp)
target_link_libraries(logger
  datetime
)

# Datetime
ADD_LIBRARY(exception  STATIC exception.cpp)

# Datetime
ADD_LIBRARY(terminate  STATIC terminate.cpp)

# Yaml_loader
include_directories(
  ${YAML_PATH}/include
)
ADD_LIBRARY(yaml_loader STATIC yaml_loader.cpp)
target_link_libraries(yaml_loader
  yaml-cpp
  terminate
)
