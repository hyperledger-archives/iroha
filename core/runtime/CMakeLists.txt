ADD_LIBRARY(runtime STATIC runtime.cpp)

target_link_libraries(runtime
    scheme
)