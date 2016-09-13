
SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -O0 -std=c++11")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/peer)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(YAML_PATH "${PROJECT_SOURCE_DIR}/vendor/yaml-cpp")

include_directories(${YAML_PATH}/include)
link_directories(${YAML_PATH}/build)

ADD_LIBRARY(yaml-loader STATIC yaml.cpp)
target_link_libraries(yaml-loader
  yaml-cpp
)
