
# Use for remove "error: cast from 'const iroha::Signature*' to 'flatbuffers::uoffset_t {aka unsigned int}' loses precision"
set(CMAKE_CXX_FLAGS "-g -std=c++1y -Wall -fPIC -fpermissive")

link_directories(
    ${CRYPTO_PATH}
    /usr/local/lib
)

ADD_LIBRARY(connection_with_grpc_flatbuffer STATIC
  connection_with_grpc_flatbuffer.cpp
)

target_link_libraries(connection_with_grpc_flatbuffer
  flatbuffers
  endpoint_fbs
  config_manager
)
