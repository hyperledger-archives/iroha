add_library(ametsuchi
    impl/flat_file/flat_file.cpp

    impl/storage_impl.cpp
    impl/temporary_wsv_impl.cpp
    impl/mutable_storage_impl.cpp

    impl/postgres_wsv_query.cpp
    )

target_link_libraries(ametsuchi
    rxcpp
    optional
    pqxx
    cpp_redis
    )