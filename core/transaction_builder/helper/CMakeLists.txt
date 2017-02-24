SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++1y -fPIC")

link_directories(
  ${PROJECT_BINARY_DIR}/lib
)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
)

ADD_LIBRARY(create_objects_helper STATIC
  create_objects_helper.cpp
)

target_link_libraries(create_objects_helper
  event_with_grpc
)
