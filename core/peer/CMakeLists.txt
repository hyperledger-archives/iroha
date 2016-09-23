
SET(YAML_PATH "${PROJECT_SOURCE_DIR}/core/vendor/yaml-cpp")
SET(AERON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/Aeron")

include_directories(
  ${AERON_PATH}/aeron-client/src/main/cpp
)
link_directories(
  ${AERON_PATH}/cppbuild/Debug/lib
)

ADD_LIBRARY(connection STATIC connection.cpp)
target_link_libraries(connection
  aeron_client
  logger
)

