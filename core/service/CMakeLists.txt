ADD_LIBRARY(executor STATIC
    executor.cpp
)
target_link_libraries(executor
    config_manager
    peer_service
    core_repository
)

ADD_LIBRARY(izanami STATIC
    izanami.cpp
)

target_link_libraries(izanami
    hash
    thread_pool
    config_manager
    peer_service
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
    izanami
    config_manager
    connection_with_grpc
    signature
    thread_pool
    executor
    merkle_transaction_repository
    transaction_repository
    validator
)