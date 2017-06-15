add_library(ordering STATIC queue.cpp observer.cpp)

target_link_libraries(ordering
  peer_service
  command_service
  ordering_service
  consensus_service
)