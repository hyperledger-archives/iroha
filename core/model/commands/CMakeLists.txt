SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
)

ADD_LIBRARY(commands STATIC
  command.cpp
)

target_link_libraries(commands
  objects
  domain_repository
  logger
)