set(CMAKE_BUILD_TYPE Debug)

link_directories(${ED25519_PATH})

ADD_LIBRARY(sumeragi STATIC
  sumeragi.cpp
  ${KECCAK_PATH}/SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c
  ${KECCAK_PATH}/Constructions/KeccakSponge.c
  ${KECCAK_PATH}/Modes/SimpleFIPS202.c
)
target_link_libraries(sumeragi
  crypto
)
