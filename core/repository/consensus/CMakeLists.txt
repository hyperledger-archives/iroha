SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(MESSAGE_PACK_PATH ${PROJECT_SOURCE_DIR}/core/vendor/msgpack-c)

include_directories(
  ${MESSAGE_PACK_PATH}/include
)
link_directories(
  ${MESSAGE_PACK_PATH}
  ${PROJECT_BINARY_DIR}/lib
)

ADD_LIBRARY(event_repository STATIC event_repository.cpp)
target_link_libraries(event_repository
  
  convert_string_with_message_pack
  world_state_repo_with_level_db
)  

ADD_LIBRARY(merkle_transaction_repository STATIC merkle_transaction_repository.cpp)
target_link_libraries(merkle_transaction_repository  
  convert_string_with_message_pack
  world_state_repo_with_level_db
)  

ADD_LIBRARY(unconfirmed_transaction_repository STATIC unconfirmed_transaction_repositoryy.cpp)
target_link_libraries(unconfirmed_transaction_repository
  convert_string_with_message_pack
  world_state_repo_with_level_db
)  
