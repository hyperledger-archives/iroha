SET(MESSAGE_PACK_PATH ${PROJECT_SOURCE_DIR}/core/vendor/msgpack-c)

include_directories(
  ${MESSAGE_PACK_PATH}/include
)
link_directories(
  ${MESSAGE_PACK_PATH}
)

ADD_LIBRARY(domain_repository STATIC account_repository.cpp)
target_link_libraries(domain_repository
  msgpackc
  exception
  # connect infra
  world_state_repo_with_level_db
)  
