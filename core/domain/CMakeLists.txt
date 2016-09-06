
set(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++14")

SET(CMAKE_SHARED_LINKER_FLAGS "-L../vendor/leveldb/out-static -lsnappy -lleveldb")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/peer)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(LEVELDB_PATH "../vendor/leveldb")

include_directories(${LEVELDB_PATH}/include)

ADD_LIBRARY(libentityrepo STATIC entityRepository.cpp)
