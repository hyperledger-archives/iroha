SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


SET(LEVELDB_PATH ${PROJECT_SOURCE_DIR}/core/vendor/leveldb)

include_directories(
  ${LEVELDB_PATH}/include
)


ADD_LIBRARY(merkle_transaction_repository STATIC merkle_transaction_repository.cpp)
target_link_libraries(merkle_transaction_repository
  world_state_repo_with_level_db
  exception
)  

