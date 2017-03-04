ADD_LIBRARY(executor STATIC
    executor.cpp
)

ADD_LIBRARY(izanami STATIC
    izanami.cpp
)

target_link_libraries(izanami
    signature
    executor
    merkle_transaction_repository
)
