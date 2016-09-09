set(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++14")

SET(CMAKE_SHARED_LINKER_FLAGS "-lpthread -L${PROJECT_BINARY_DIR}/lib")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)

SET(CRYPTO_PATH  "${PROJECT_SOURCE_DIR}/core/crypto")
SET(ED25519_PATH "${PROJECT_SOURCE_DIR}/core/vendor/ed25519")

set(GTEST_INCLUDE_DIR ${PROJECT_BINARY_DIR}/gtest/src/googletest/googletest/include)

include_directories(
  ${CRYPTO_PATH}
  ${GTEST_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/core/vendor/ed25519/src
)
link_directories(${ED25519_PATH})

add_executable(base64-test base64_test.cpp)
target_link_libraries(base64-test base64
  ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread)
add_test(
  NAME base64-test
  COMMAND $<TARGET_FILE:base64-test>
)

add_executable(signature-test signature_test.cpp)
target_link_libraries(signature-test signature
  ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread)
add_test(
  NAME signature-test
  COMMAND $<TARGET_FILE:signature-test>
)
