cmake_minimum_required(VERSION 2.8)
PROJECT(iroha CXX)

ENABLE_LANGUAGE(C)
find_package(Threads)

add_subdirectory(core)


if(NOT without-test)
  enable_testing()
  include(test/gtest.cmake)
  add_subdirectory(test)
endif()


