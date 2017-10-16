add_library(iroha_amount
    amount.cpp
    )

target_link_libraries(iroha_amount
    optional
    logger
    )
target_include_directories(iroha_amount PUBLIC
    ${Boost_INCLUDE_DIRS}
    )
