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

# Base64 Test
add_executable(base64_test base64_test.cpp)
target_link_libraries(base64_test base64
  ssl
  crypto
  ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread)
add_test(
  NAME base64_test
  COMMAND $<TARGET_FILE:base64_test>
)

# Singature Test
add_executable(signature_test signature_test.cpp)
target_link_libraries(signature_test signature
  base64
  ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread)
add_test(
  NAME signature-test
  COMMAND $<TARGET_FILE:signature_test>
)
