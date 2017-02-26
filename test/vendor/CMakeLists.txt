SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

include_directories(
  ${TP_DIR}
  ${PROJECT_SOURCE_DIR}/core
)

add_executable(thread_pool_test
  thread_pool_test.cpp
)

target_link_libraries(thread_pool_test
  external_gtest
)

add_test(
  NAME thread_pool_test
  COMMAND $<TARGET_FILE:thread_pool_test>
)

# json test
add_executable(json_test
    json_test.cpp
)

target_link_libraries(json_test
   external_gtest
)
add_test(
  NAME json_test
  COMMAND $<TARGET_FILE:json_test>
)
