add_library(crypto STATIC
        base64.h
        hash.h
        keypair.h
        crypto.cc
        crypto.h
        )

target_link_libraries(crypto PUBLIC
        keccak
        ed25519
        optional
        any
        )

