SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

link_directories(
  ${PROJECT_BINARY_DIR}/lib
)

add_library(event_repository static
    event_repository.cpp
)