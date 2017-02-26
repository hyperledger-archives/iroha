set(CMAKE_BUILD_TYPE Debug)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)


add_executable(convertor_test
    convertor_test.cpp
)
target_link_libraries(convertor_test
   convertor
   signature
   datetime
   hash
   base64
   world_state_repo_with_level_db
   event_repository
   grpc++
   grpc++_reflection
   protobuf
   pthread
   dl
   external_gtest
)
add_test(
    NAME convertor_test
    COMMAND $<TARGET_FILE:convertor_test>
)
