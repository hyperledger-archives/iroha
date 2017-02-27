cmake_minimum_required(VERSION 3.3.0 FATAL_ERROR)

PROJECT(iroha CXX)

ENABLE_LANGUAGE(C)
find_package(Threads)

SET(CMAKE_CXX_FLAGS "-g -std=c++1y -Wall -fPIC")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wextra -O0")
SET(CMAKE_SHARED_LINKER_FLAGS "-lpthread -lssl")

add_subdirectory(core)
add_subdirectory(peer)
add_subdirectory(tools)
add_subdirectory(smart_contract)

if(NOT without-test)
  enable_testing()
  include(test/gtest.cmake)
  add_subdirectory(test)
endif()