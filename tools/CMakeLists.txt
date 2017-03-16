SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

###########################
#      make sumeragi      #
###########################
# make config
add_executable(make_sumeragi make_sumeragi.cpp)
target_link_libraries(make_sumeragi
    base64
    signature
    json
)

###########################
#    issue transaction    #
###########################

# automatic generation source files by using templates
# execute_process(COMMAND bash "-c" "cd $IROHA_HOME/tools/helper; ./allbuild.sh")
add_custom_command(
    OUTPUT issue_autobuild.out
    COMMAND bash "-c" "cd $IROHA_HOME/tools/helper; ./allbuild.sh"
)

add_custom_target(issue_autobuild_out
    DEPENDS issue_autobuild.out
)

add_executable(issue_transaction issue_transaction.cpp)

target_link_libraries(issue_transaction
    json
    config_manager
    peer_service
    logger
    event_with_grpc
    create_objects_helper
    connection_with_grpc
    transaction_repository
)

add_dependencies(issue_transaction issue_autobuild_out)

# make config
add_executable(send_izanami_test send_izanami.cpp)
target_link_libraries(send_izanami_test
    peer_service
    pthread
)