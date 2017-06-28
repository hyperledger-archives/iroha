ADD_LIBRARY(stateful_validator STATIC validator.cpp)

target_link_libraries(stateful_validator
    schema
    rxcpp
    optional
)