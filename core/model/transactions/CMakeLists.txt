set(JSON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/json")

include_directories(
        ${JSON_PATH}/src
)

set(CMAKE_BUILD_TYPE Debug)

ADD_LIBRARY(transfer_transaction STATIC
  transfer_transaction.cpp
)
target_link_libraries(transfer_transaction
    hash
)