add_subdirectory(objects)

ADD_LIBRARY(transaction STATIC
  transaction.cpp
)

target_link_libraries(transaction
  objects
  datetime
  hash
  logger
)
