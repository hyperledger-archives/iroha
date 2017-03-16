ADD_LIBRARY(executor STATIC
    executor.cpp
)
target_link_libraries(executor
    config_manager
    core_repository
)

ADD_LIBRARY(izanami STATIC
    izanami.cpp
)

target_link_libraries(izanami
    hash
    thread_pool
    config_manager
    executor
    event_with_grpc
    transaction_repository
)

ADD_LIBRARY(peer_service STATIC
    peer_service.cpp
)

target_link_libraries(peer_service
    json
    exception
    logger
    config_manager
)