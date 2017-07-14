add_library(stateless_validator
    impl/stateful_validator_impl.cpp
    )

target_link_libraries(stateless_validator
    optional
    ed25519
    rxcpp
    model
    )
