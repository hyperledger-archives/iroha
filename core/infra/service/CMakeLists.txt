
ADD_LIBRARY(http_client STATIC
    http_client.cpp
)

target_link_libraries(http_client
    ssl
    crypto
    logger
    json
)


ADD_LIBRARY(flatbuffer_service STATIC
    flatbuffer_service.cpp
)