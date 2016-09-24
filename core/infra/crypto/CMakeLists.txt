
SET(KECCAK_PATH  "${PROJECT_SOURCE_DIR}/core/vendor/KeccakCodePackage")
SET(ED25519_PATH "${PROJECT_SOURCE_DIR}/core/vendor/ed25519")

# Hash

# Bad solution (T A T)
execute_process(COMMAND "cd ${PROJECT_SOURCE_DIR}/core/infra/crypto; make")

# Why not running??
#[[
add_definitions(
  -DKeccakP200_excluded
  -DKeccakP400_excluded
  -DKeccakP800_excluded
)

include_directories(${KECCAK_PATH}/bin/generic64/libkeccak.a.headers)
link_directories(${ED25519_PATH})
link_directories(
  ${KECCAK_PATH}/bin/generic64
  ${PROJECT_SOURCE_DIR}/core/infra/crypto
)
message(status ${PROJECT_SOURCE_DIR}/core/infra/crypto)

ADD_LIBRARY(iroha_hash STATIC
  ${KECCAK_PATH}/SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c
  ${KECCAK_PATH}/Constructions/KeccakSponge.c
  ${KECCAK_PATH}/Modes/SimpleFIPS202.c
  hash.cpp
)
target_link_libraries(iroha_hash
  keccak
)

ADD_EXECUTABLE(unit
  unit.cpp  
  ${KECCAK_PATH}/SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c
  ${KECCAK_PATH}/Constructions/KeccakSponge.c
  ${KECCAK_PATH}/Modes/SimpleFIPS202.c
)
]]

# Base64
ADD_LIBRARY(base64 STATIC
  base64.cpp
)

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