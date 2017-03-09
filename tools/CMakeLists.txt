SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# make config
add_executable(make_sumeragi make_sumeragi )
target_link_libraries(make_sumeragi
    base64
    signature
    json
)

add_executable(issue_transaction issue_transaction.cpp )
target_link_libraries(issue_transaction
    json
    config_manager
    logger
    event_with_grpc
    create_objects_helper
    connection_with_grpc
)
