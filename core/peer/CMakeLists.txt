
SET(YAML_PATH "${PROJECT_SOURCE_DIR}/core/vendor/yaml-cpp")
SET(AERON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/Aeron")

include_directories(
#  ${YAML_PATH}/include
  ${AERON_PATH}/aeron-client/src/main/cpp
)
link_directories(
  ${AERON_PATH}/cppbuild/Debug/lib
  #${YAML_PATH}/build
)

ADD_LIBRARY(connection STATIC connection.cpp)
target_link_libraries(connection
  aeron_client
  logger
)

