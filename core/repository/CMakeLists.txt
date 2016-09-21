
SET(CMAKE_SHARED_LINKER_FLAGS "-L../vendor/leveldb/out-static -lmsgpack -lsnappy -lleveldb")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/peer)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(LEVELDB_PATH "../vendor/leveldb")

include_directories(${LEVELDB_PATH}/include)

ADD_LIBRARY(txRepo STATIC TransactionRepository.cpp)

