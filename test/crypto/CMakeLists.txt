set(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++14")

SET(CMAKE_SHARED_LINKER_FLAGS "-lpthread -lssl -L${PROJECT_BINARY_DIR}/lib")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(CRYPTO_PATH "../../core/crypto")

ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIR ${source_dir}/include)
set(INC ${PROJECT_BINARY_DIR}/gtest/src/googletest/googletest/include)

include_directories(
  ${CRYPTO_PATH}
  ${GTEST_INCLUDE_DIR}
  ${INC}
)

add_executable(base64-test base64_test.cpp)
target_link_libraries(base64-test
  base64
  ${GTEST_LIBS_DIR}/libgtest.a
  ${GTEST_LIBS_DIR}/libgtest_main.a
  pthread
)

add_test(
  NAME base64-test
  COMMAND $<TARGET_FILE:base64-test>
)

