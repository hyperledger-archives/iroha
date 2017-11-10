add_library(shared_model_proto_backend
    impl.cpp
    )

target_include_directories(shared_model_proto_backend PUBLIC
    ${PROJECT_SOURCE_DIR}/schema)

target_link_libraries(shared_model_proto_backend
    schema
    model_interfaces
    )
