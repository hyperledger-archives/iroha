add_library(model
    model.cpp
    )

target_link_libraries(model
    schema
    optional
    rxcpp
    )
