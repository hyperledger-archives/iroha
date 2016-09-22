
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SET(YAML_PATH "${PROJECT_SOURCE_DIR}/core/vendor/yaml-cpp")

set(CMAKE_CXX_FLAGS "-Wall -std=c++1y")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wextra -O0")

include_directories(${PROJECT_SOURCE_DIR}/core/server)
link_directories(${YAML_PATH}/build)

add_executable(iroha-main
  main.cpp
)
target_link_libraries(iroha-main
  http_server
  pthread
)

