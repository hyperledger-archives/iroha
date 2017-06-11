#ADD_LIBRARY(izanami STATIC
#    izanami.cpp
#)

#target_link_libraries(izanami
#    hash
#    thread_pool
#    config_manager
#    peer_service
#    executor
#    connection_with_grpc
#    transaction_repository
#)

ADD_LIBRARY(membership_service STATIC
    peer_service.cpp
)

target_link_libraries(membership_service
    timer
    exception
    logger
    config_manager
    cache_map
    flatbuffer_service
    connection_with_grpc_flatbuffer
)