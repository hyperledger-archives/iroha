
set(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++11")

SET(MSGPACK_PATH "${PROJECT_SOURCE_DIR}/core/vendor/msgpack-c")

include_directories(${KECCAK_PATH}/include)


