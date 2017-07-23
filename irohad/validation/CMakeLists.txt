add_subdirectory(stateless)

add_library(stateful_validator
    impl/stateful_validator_impl.cpp
    )

target_link_libraries(stateful_validator
    optional
    ed25519
    rxcpp
    model
    )

add_library(chain_validator
    impl/chain_validator_impl.cpp)

target_link_libraries(chain_validator
    optional
    ed25519
    rxcpp
    model
    )