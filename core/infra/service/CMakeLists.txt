set(JSON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/json")

include_directories(
    ${JSON_PATH}/src
    ${PROJECT_SOURCE_DIR}/core
)

<<<<<<< HEAD

ADD_LIBRARY(executor STATIC
    executor.cpp
)


=======
ADD_LIBRARY(http_client STATIC
    http_client.cpp
)


target_link_libraries(http_client
    ssl
    crypto
    logger
)

>>>>>>> feature/http-client
