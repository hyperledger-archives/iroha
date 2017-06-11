cmake_minimum_required(VERSION 2.5)
PROJECT(iroha C CXX)

SET(CMAKE_CXX_FLAGS "-std=c++1y -Wall -fPIC")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG   "-g -Wextra -Wno-unused-parameter -O0")

set(IROHA_SCHEMA_DIR "${PROJECT_SOURCE_DIR}/schema")

include_directories(
  ${PROJECT_SOURCE_DIR}/core
  ${PROJECT_SOURCE_DIR}/core/infra/ametsuchi/include
  ${IROHA_SCHEMA_DIR}
)

add_subdirectory(schema)
add_subdirectory(core)

link_directories(${PROJECT_BINARY_DIR}/lib)
