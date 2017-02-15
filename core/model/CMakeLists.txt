add_subdirectory(commands)
add_subdirectory(objects)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
  "."
)

add_library(transaction STATIC
  transaction.cpp
)

target_link_libraries(transaction
  objects
  datetime
  hash
  logger
)