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

#ADD_LIBRARY(peer_service STATIC
#    monitor.cpp
#    change_state.cpp
#    self_state.cpp
#    synchronizer/synchronizer.cpp
#)
#
#target_link_libraries(peer_service
#    timer
#    time64
#    logger
#    crypto
#    schema
#)

add_library(peer_service
        node.hpp
        service.cpp
        service.hpp
        connection_to.cpp
        connection_to.hpp
        )
target_link_libraries(peer_service
        uvw
        peer_service_grpc
        lookup3
        )
