set(JSON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/json")

include_directories(
    ${JSON_PATH}/src
    ${PROJECT_SOURCE_DIR}/core
)

ADD_LIBRARY(http_client STATIC
    http_client.cpp
)


target_link_libraries(http_client
    ssl
    crypto
    logger
)

