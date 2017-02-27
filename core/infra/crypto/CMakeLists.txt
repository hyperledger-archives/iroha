cmake_minimum_required(VERSION 2.8)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
  )

# Base64
ADD_LIBRARY(base64 STATIC base64.cpp)
target_include_directories(base64 INTERFACE ${PROJECT_SOURCE_DIR}/core)

# Signature
ADD_LIBRARY(signature STATIC signature.cpp)
target_link_libraries(signature
  ed25519
  base64
)

# Hash
ADD_LIBRARY(hash STATIC hash.cpp)
target_link_libraries(hash
  keccak
)
