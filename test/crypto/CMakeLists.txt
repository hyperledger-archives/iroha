set(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++14")

SET(CMAKE_SHARED_LINKER_FLAGS "-lpthread -lssl -L${PROJECT_BINARY_DIR}/lib ")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/peer)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


SET(CRYPTO_PATH "../../core/crypto")

SET(CMAKE_SHARED_LINKER_FLAGS "-lpthread -lssl -led25519 -L../vendor/ed25519")
include_directories(${CRYPTO_PATH})


add_executable(unit_test_base64 base64_test.cpp)

