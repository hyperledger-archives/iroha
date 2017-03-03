include_directories(
    "${PROJECT_SOURCE_DIR}/core"
)

ADD_LIBRARY(executor STATIC
    executor.cpp
)

