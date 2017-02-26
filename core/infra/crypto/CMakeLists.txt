cmake_minimum_required(VERSION 2.8)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
  )

# Base64
ADD_LIBRARY(base64 STATIC base64.cpp)

# Signature
ADD_LIBRARY(signature STATIC signature.cpp)
target_link_libraries(signature
  external_ed25519   # declared in dependencies.cmake
  base64
)

# Hash
ADD_LIBRARY(hash STATIC hash.cpp)
target_link_libraries(hash
  external_keccak    # declared in dependencies.cmake
)
