SET(MESSAGE_PACK_PATH ${PROJECT_SOURCE_DIR}/core/vendor/msgpack-c)

include_directories(
  ${MESSAGE_PACK_PATH}/include
)
link_directories(
  ${MESSAGE_PACK_PATH}
)

ADD_LIBRARY(peer_service STATIC peer_service.cpp)
target_link_libraries(peer_service
  msgpackc
  exception
  yaml_loader
  # connect infra
  world_state_repo_with_level_db
)  
