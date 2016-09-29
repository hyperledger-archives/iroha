add_subdirectory(domain)
add_subdirectory(consensus)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(MESSAGE_PACK_PATH ${PROJECT_SOURCE_DIR}/core/vendor/msgpack-c)
SET(LEVELDB_PATH ${PROJECT_SOURCE_DIR}/core/vendor/leveldb)

include_directories(
  ${LEVELDB_PATH}/include
  ${MESSAGE_PACK_PATH}/include
)

link_directories(
  ${MESSAGE_PACK_PATH}
  ${LEVELDB_PATH}/out-static
)

ADD_LIBRARY(world_state_repo_with_level_db STATIC world_state_repository_with_level_db.cpp)
target_link_libraries(world_state_repo_with_level_db
  msgpackc
  snappy
  leveldb
  exception
)  
