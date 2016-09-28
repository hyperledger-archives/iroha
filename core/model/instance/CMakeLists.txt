SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# This is infra knowledge
SET(MESSAGE_PACK_PATH ${PROJECT_SOURCE_DIR}/core/vendor/msgpack-c)
include_directories(${MESSAGE_PACK_PATH}/include)

# All domain instance is in libdomain.a
ADD_LIBRARY(domain  STATIC
  account.cpp
  sample_asset.cpp
)
