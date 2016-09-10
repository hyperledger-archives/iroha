
set(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -O0 -std=c++11")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/peer)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(CROW_PATH "../vendor/crow")

include_directories(${CROW_PATH}/include)

ADD_LIBRARY(httpserver STATIC http_server.cpp)
