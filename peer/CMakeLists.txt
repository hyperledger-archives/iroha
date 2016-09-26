
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SET(YAML_PATH  "${PROJECT_SOURCE_DIR}/core/vendor/yaml-cpp")
SET(AERON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/Aeron")

include_directories(
  ${PROJECT_SOURCE_DIR}/core/server
  ${PROJECT_SOURCE_DIR}/core/peer

  ${YAML_PATH}/include
  ${AERON_PATH}/aeron-client/src/main/cpp
)

link_directories(
  ${YAML_PATH}/build
  ${AERON_PATH}/cppbuild/Debug/lib
)

add_executable(iroha-main
  main.cpp
)
target_link_libraries(iroha-main
  sumeragi
  logger
  yaml_loader
  http_server_with_crow
  connection_with_aeron
  pthread
)

