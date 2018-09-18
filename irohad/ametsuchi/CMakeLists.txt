add_library(ametsuchi INTERFACE)

target_include_directories(ametsuchi
    INTERFACE interface
    )

target_link_libraries(ametsuchi
    INTERFACE
        rxcpp
        libs_common
        common
        shared_model_interfaces
    )

add_library(ametsuchi_postgres
    postgres/ametsuchi/impl/storage_impl.cpp
    postgres/ametsuchi/impl/temporary_wsv_impl.cpp
    postgres/ametsuchi/impl/mutable_storage_impl.cpp
    postgres/ametsuchi/impl/postgres_wsv_query.cpp
    postgres/ametsuchi/impl/postgres_wsv_command.cpp
    postgres/ametsuchi/impl/peer_query_wsv.cpp
    postgres/ametsuchi/impl/postgres_block_query.cpp
    postgres/ametsuchi/impl/postgres_command_executor.cpp
    postgres/ametsuchi/impl/postgres_block_index.cpp
    postgres/ametsuchi/impl/postgres_ordering_service_persistent_state.cpp
    postgres/ametsuchi/impl/wsv_restorer_impl.cpp
    postgres/ametsuchi/impl/postgres_options.cpp
    )

target_include_directories(ametsuchi_postgres
    PUBLIC postgres
    )

target_link_libraries(ametsuchi_postgres
    PUBLIC
        ametsuchi
        logger
        query_execution
        shared_model_stateless_validation
        SOCI::core
        SOCI::postgresql
        flat_file
    )

target_compile_definitions(ametsuchi_postgres
    PRIVATE SOCI_USE_BOOST HAVE_BOOST
    )

add_library(flat_file
    flat_file/ametsuchi/impl/flat_file/flat_file.cpp
    )

target_include_directories(flat_file
    PUBLIC flat_file
)

target_link_libraries(flat_file
    PUBLIC logger ametsuchi
    PRIVATE boost
)


