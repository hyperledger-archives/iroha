# Torii Test
add_executable(torii_async_test
  torii_async_test.cpp
  )
target_link_libraries(torii_async_test
  command_service
  command_client
  server_runner
  gtest
  )
add_test(
  NAME torii_async_test
  COMMAND $<TARGET_FILE:torii_async_test>
)
