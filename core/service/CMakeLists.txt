ADD_LIBRARY(executor STATIC
    executor.cpp
)

target_link_libraries(executor
    logger
    flatbuffers
)
