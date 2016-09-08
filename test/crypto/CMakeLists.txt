set(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++14")

SET(CMAKE_SHARED_LINKER_FLAGS "-lpthread -lssl -L${PROJECT_BINARY_DIR}/lib")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(CRYPTO_PATH "../../core/crypto")

include_directories(${CRYPTO_PATH})

add_executable(base64-test base64_test.cpp)
target_link_libraries(base64-test
  testutil
  base64
)

add_test(
  NAME base64-test
  COMMAND $<TARGET_FILE:base64-test>
)

