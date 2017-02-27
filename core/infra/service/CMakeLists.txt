set(JSON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/json")

include_directories(
    ${JSON_PATH}/src
    ${PROJECT_SOURCE_DIR}/core
)


ADD_LIBRARY(executor STATIC
    executor.cpp
)


