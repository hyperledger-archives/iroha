set(CMAKE_BUILD_TYPE Debug)

link_directories(${ED25519_PATH})

ADD_LIBRARY(sumeragi STATIC
  sumeragi.cpp
)
target_link_libraries(sumeragi
  crypto
)
