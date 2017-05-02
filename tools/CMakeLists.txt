SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)



###########################
#      Sumeragi test      #
###########################

add_executable(test_sumeragi
    test_sumeragi.cpp
)

target_link_libraries(test_sumeragi
    connection_with_grpc_flatbuffer
)

###########################
#      Ametsuchi test      #
###########################

add_executable(check_ametsuchi
    check_ametsuchi.cpp
)

target_link_libraries(check_ametsuchi
    connection_with_grpc_flatbuffer
)