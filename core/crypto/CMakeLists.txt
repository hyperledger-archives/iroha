set(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++14")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++11")

SET(CMAKE_SHARED_LINKER_FLAGS "-lpthread -lssl")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/peer)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(KECCAK_PATH  "${PROJECT_SOURCE_DIR}/core/vendor/KeccakCodePackage")
SET(ED25519_PATH "${PROJECT_SOURCE_DIR}/core/vendor/ed25519")

include_directories(${KECCAK_PATH}/)
include_directories(${KECCAK_PATH}/Common/)
include_directories(${KECCAK_PATH}/SnP/)
include_directories(${KECCAK_PATH}/Constructions/)
include_directories(${KECCAK_PATH}/Modes/)
include_directories(${KECCAK_PATH}/SnP/KeccakP-200/Compact)
include_directories(${KECCAK_PATH}/SnP/KeccakP-400/Reference)
include_directories(${KECCAK_PATH}/SnP/KeccakP-800/Compact)
include_directories(${KECCAK_PATH}/SnP/KeccakP-1600/Optimized64/ufull)
include_directories(${KECCAK_PATH}/SnP/KeccakP-1600/Optimized/)
include_directories(${KECCAK_PATH}/SnP/KeccakP-1600/Optimized64/)

link_directories(${ED25519_PATH})

ADD_LIBRARY(hash STATIC
  Hash.cpp
  ${KECCAK_PATH}/SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c
  ${KECCAK_PATH}/Constructions/KeccakSponge.c
  ${KECCAK_PATH}/Modes/SimpleFIPS202.c
)

include_directories(${ED25519_PATH}/src)

ADD_LIBRARY(signature STATIC
  Signature.cpp
)
target_link_libraries(signature
  ed25519
)
ADD_LIBRARY(base64 STATIC Base64.cpp)
target_link_libraries(base64
  ssl
  crypto)