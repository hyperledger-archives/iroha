ADD_LIBRARY(stateful_validator STATIC
    validator.cpp
    impl/stub_command_validator.cpp)

target_link_libraries(stateful_validator
    schema
    ametsuchi
    rxcpp
    optional
    )