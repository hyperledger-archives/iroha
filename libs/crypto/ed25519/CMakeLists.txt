add_library(ed25519 SHARED
    add_scalar.c
    fe.c
    ge.c
    key_exchange.c
    keypair.c
    sc.c
    seed.c
    sha3.c
    sign.c
    verify.c
    )
target_include_directories(ed25519 INTERFACE .)
