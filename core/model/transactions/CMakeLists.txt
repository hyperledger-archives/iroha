set(CMAKE_BUILD_TYPE Debug)

ADD_LIBRARY(transfer_transaction STATIC
  transfer_transaction.cpp
)
target_link_libraries(transfer_transaction
    hash
)