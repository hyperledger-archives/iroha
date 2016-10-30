set(JSON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/json")

include_directories(
        ${JSON_PATH}/src
)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ADD_LIBRARY(sumeragi STATIC
  sumeragi.cpp
)
target_link_libraries(sumeragi
  peer_service_with_json
  signature
  event_repository
  merkle_transaction_repository
)