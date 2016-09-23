set(CMAKE_BUILD_TYPE Debug)

ADD_LIBRARY(sumeragi STATIC
  sumeragi.cpp
)
target_link_libraries(sumeragi
  crypto
  txRepo
)
