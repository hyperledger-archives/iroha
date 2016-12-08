add_subdirectory(objects)
add_subdirectory(commands)

ADD_LIBRARY(transaction STATIC
  transaction.cpp
)

target_link_libraries(transaction
  domain_repository
  commands
  objects
)