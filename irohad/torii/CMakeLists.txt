add_library(torii
    impl/responder.cpp
    )

target_link_libraries(torii PUBLIC
    dao
    )