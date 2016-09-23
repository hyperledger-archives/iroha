
SET(MESSAGE_PACK_PATH ${PROJECT_SOURCE_DIR}/core/vendor/msgpack-c)
SET(LEVELDB_PATH ${PROJECT_SOURCE_DIR}/core/vendor/leveldb)

include_directories(
  ${LEVELDB_PATH}/include
  ${MESSAGE_PACK_PATH}/include
)

message(status ${LEVELDB_PATH})
link_directories(
  ${MESSAGE_PACK_PATH}
  ${LEVELDB_PATH}/out-static
)

ADD_LIBRARY(txRepo STATIC merkle_transaction_repository.cpp)
target_link_libraries(txRepo
  msgpackc
  snappy
  leveldb
)  
