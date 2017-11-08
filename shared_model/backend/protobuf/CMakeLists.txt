add_library(proto commands/proto_command.cpp commands/proto_add_asset_quantity.cpp)

target_include_directories(proto PUBLIC ${PROJECT_SOURCE_DIR}/schema)

target_link_libraries(proto schema model_interfaces)
