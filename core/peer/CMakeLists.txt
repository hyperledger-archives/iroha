
set(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -O0 -std=c++14")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/peer)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(AERON_PATH "../vendor/Aeron")

include_directories(${AERON_PATH}/cppbuild/Debug/lib)
