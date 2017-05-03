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