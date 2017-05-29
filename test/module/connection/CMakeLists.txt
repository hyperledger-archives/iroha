add_executable(grpc_test
    grpc_test.cpp
)
target_link_libraries(grpc_test
    gtest
    endpoint_fbs
    signature
    flatbuffers
)
add_test(
    NAME grpc_test
    COMMAND $<TARGET_FILE:grpc_test>
)
