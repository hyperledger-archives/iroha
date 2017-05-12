########################################################################################
# CacheMapTEST
########################################################################################
add_executable(cache_map_test cache_map_test.cpp)
target_link_libraries(cache_map_test
  gtest
  cache_map
)
add_test(
  NAME cache_map_test
  COMMAND $<TARGET_FILE:cache_map_test>
)
########################################################################################
# exceptionTEST
########################################################################################
add_executable(exception_test exception_test.cpp)
target_link_libraries(exception_test
  gtest
  exception
)
add_test(
  NAME exception_test
  COMMAND $<TARGET_FILE:exception_test>
)
########################################################################################
# loggerTEST
########################################################################################
add_executable(logger_test logger_test.cpp)
target_link_libraries(logger_test
  gtest
  logger
)
add_test(
  NAME logger_test
  COMMAND $<TARGET_FILE:logger_test>
)
