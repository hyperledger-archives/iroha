add_library(hash STATIC
        hash.cpp
        hash.hpp
        )

target_link_libraries(hash PUBLIC
        keccak
        )




add_library(crypto STATIC
        crypto.hpp
        ed25519_impl.cpp
        )
target_link_libraries(crypto PUBLiC
        ed25519
        hash
        )