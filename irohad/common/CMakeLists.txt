add_library(config config.cpp)
target_link_libraries(config
    rapidjson
)

add_library(blob_converter impl/blob_converter.cpp)
