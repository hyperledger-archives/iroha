set(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++11")

SET(CMAKE_SHARED_LINKER_FLAGS "-lpthread -lssl")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

SET(CRYPTO_PATH  "${PROJECT_SOURCE_DIR}/core/crypto")
SET(ED25519_PATH "${PROJECT_SOURCE_DIR}/core/vendor/ed25519")

set(GTEST_INCLUDE_DIR ${PROJECT_BINARY_DIR}/gtest/src/googletest/googletest/include)

include_directories(
  ${CRYPTO_PATH}
  ${GTEST_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/core/vendor/ed25519/src
)
link_directories(
  ${ED25519_PATH}
  ${PROJECT_BINARY_DIR}/lib
)

add_executable(base64-test Base64Test.cpp)
target_link_libraries(base64-test base64
  ssl
  crypto
  ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread)
add_test(
  NAME base64-test
  COMMAND $<TARGET_FILE:base64-test>
)

add_executable(signature-test SignatureTest.cpp)
target_link_libraries(signature-test signature
  base64
  ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread)
add_test(
  NAME signature-test
  COMMAND $<TARGET_FILE:signature-test>
)
