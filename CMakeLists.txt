cmake_minimum_required(VERSION 2.8)
PROJECT(iroha C CXX)

SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++1y")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wextra -O0")
SET(CMAKE_SHARED_LINKER_FLAGS "-lpthread -lssl")

include("dependencies.cmake")

add_subdirectory(core)
add_subdirectory(peer)
add_subdirectory(tools)

if(NOT without-test)
  enable_testing()
  add_subdirectory(test)
endif()