#add_subdirectory(block_store)
#add_subdirectory(index)
#add_subdirectory(wsv)

add_library(storage
    #    impl/ametsuchi_impl.cpp
    #    index_mediator.cpp
    )

target_link_libraries(storage PUBLIC
    schema
    cpp_redis
    pqxx
    optional
    #    block_store
    #    index
    #    wsv
    rxcpp
    )