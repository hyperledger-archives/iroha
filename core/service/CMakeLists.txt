ADD_LIBRARY(executor STATIC
    executor.cpp
)
target_link_libraries(executor
    config_manager
)

ADD_LIBRARY(izanami STATIC
    izanami.cpp
)

target_link_libraries(izanami
    signature
    executor
    merkle_transaction_repository
    hash
)
