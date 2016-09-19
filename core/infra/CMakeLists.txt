set(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++11")

SET(CMAKE_SHARED_LINKER_FLAGS "-lpthread")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/peer)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(AERON_PATH  "${PROJECT_SOURCE_DIR}/core/vendor/Aeron")

include_directories(${AERON_PATH}/aeron-client/src/main/cpp)
link_directories(${AERON_PATH}/cppbuild/Debug/lib/)

ADD_LIBRARY(connection STATIC
  Connection.cpp
)
target_link_libraries(connection
  aeron_client
)

