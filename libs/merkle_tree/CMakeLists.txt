add_library(merkle_tree
        merkle_tree.cc
        merkle_tree.h
        )

target_link_libraries(merkle_tree PUBLIC
        crypto
        )
