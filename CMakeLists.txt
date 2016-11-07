cmake_minimum_required(VERSION 3.0)
PROJECT(iroha CXX)

ENABLE_LANGUAGE(CXX)
find_package(Threads)

SET(CMAKE_C_COMPILER "/usr/bin/gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")

SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++14")
#SET(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(-DCMAKE_CXX_COMPILER=/usr/bin/g++)
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wextra -O0")
SET(CMAKE_SHARED_LINKER_FLAGS "-lpthread -lssl")

#set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})

add_subdirectory(core)
add_subdirectory(peer)

if(NOT without-test)
  enable_testing()
  include(test/gtest.cmake)
  add_subdirectory(test)
endif()
