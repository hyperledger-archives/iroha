include_directories(
    ${PROJECT_SOURCE_DIR}/core
    ${PROJECT_SOURCE_DIR}/core/vendor/json/src
)

ADD_LIBRARY(executor STATIC
    executor.cpp
)

ADD_LIBRARY(http_client STATIC
    http_client.cpp
)


target_link_libraries(http_client
    ssl
    crypto
    logger
)
