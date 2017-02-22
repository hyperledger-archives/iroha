
include_directories(
    ${PROJECT_SOURCE_DIR}/core
    "."
)

# All domain instance is in libdomain.a
ADD_LIBRARY(objects STATIC
    object.cpp
    base_object.cpp
)
