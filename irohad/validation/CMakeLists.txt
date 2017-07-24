add_subdirectory(stateless)
add_subdirectory(chain)

add_library(stateful_validator
    impl/stateful_validator_impl.cpp
    )

target_link_libraries(stateful_validator
    optional
    ed25519
    rxcpp
    model
    )
