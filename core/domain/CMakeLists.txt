
set(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++11")

SET(CMAKE_SHARED_LINKER_FLAGS "-L../vendor/leveldb/out-static -lmsgpack -lsnappy -lleveldb") #TODO: これ必要？