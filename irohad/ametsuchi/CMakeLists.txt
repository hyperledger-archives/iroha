# here will be the code, which is responsible for connection to ametsuchi
message(STATUS "Ametsuchi")

add_library(storage
    storage.cpp storage.hpp
    )

target_link_libraries(storage PUBLIC
    ametsuchi
    )