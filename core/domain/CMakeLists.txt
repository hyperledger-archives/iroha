

set(JSON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/json")
include_directories(${JSON_PATH}/src)

# Sample Asset
ADD_LIBRARY(sample_asset  STATIC sample_asset.cpp)
target_link_libraries(sample_asset
  datetime
  logger
  datetime
)