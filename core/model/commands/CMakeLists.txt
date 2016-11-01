set(CMAKE_BUILD_TYPE Debug)

# All domain instance is in libdomain.a
ADD_LIBRARY(commands STATIC
    add.cpp
    transfer.cpp
)

target_link_libraries(commands
    hash
)