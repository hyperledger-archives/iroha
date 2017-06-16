add_subdirectory(connection)
add_library(sumeragi STATIC
  sumeragi.cpp
)
target_link_libraries(sumeragi
  schema
  crypto
  sumeragi_connection
  thread_pool
  timer
  logger
)
