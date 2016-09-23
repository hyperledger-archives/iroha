
SET(KECCAK_PATH  "${PROJECT_SOURCE_DIR}/core/vendor/KeccakCodePackage")
SET(ED25519_PATH "${PROJECT_SOURCE_DIR}/core/vendor/ed25519")

# Hash
include_directories(
  ${KECCAK_PATH}/
  ${KECCAK_PATH}/Common/
  ${KECCAK_PATH}/SnP/
  ${KECCAK_PATH}/Constructions/
  ${KECCAK_PATH}/Modes/
  ${KECCAK_PATH}/SnP/KeccakP-200/Compact
  ${KECCAK_PATH}/SnP/KeccakP-400/Reference
  ${KECCAK_PATH}/SnP/KeccakP-800/Compact
  ${KECCAK_PATH}/SnP/KeccakP-1600/Optimized64/ufull
  ${KECCAK_PATH}/SnP/KeccakP-1600/Optimized/
  ${KECCAK_PATH}/SnP/KeccakP-1600/Optimized64/
  ${KECCAK_PATH}/bin/generic64/libkeccak.a.headers
)
link_directories(${ED25519_PATH})
link_directories(${KECCAK_PATH}/bin/generic64/)
ADD_LIBRARY(hash STATIC
  hash.cpp
  ${KECCAK_PATH}/SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c
  ${KECCAK_PATH}/Constructions/KeccakSponge.c
  ${KECCAK_PATH}/Modes/SimpleFIPS202.c
)
target_link_libraries(hash
  keccak
)

# Base64
ADD_LIBRARY(base64 STATIC base64.cpp)

# Signature
include_directories(${ED25519_PATH}/src)
ADD_LIBRARY(signature STATIC
  signature.cpp
)
target_link_libraries(signature
  ed25519
  base64
  hash
)