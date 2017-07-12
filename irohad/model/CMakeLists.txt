add_library(model
    model_crypto_provider_impl.cpp
    model_hash_provider_impl.cpp
    )

target_link_libraries(model
    schema
    optional
    rxcpp
    )
