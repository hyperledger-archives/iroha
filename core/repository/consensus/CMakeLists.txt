set(JSON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/json")

include_directories(
        ${JSON_PATH}/src
)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library(event_repository STATIC
    event_repository.cpp
)

add_library(merkle_transaction_repository STATIC
    merkle_transaction_repository.cpp
)
target_link_libraries(merkle_transaction_repository
    hash
    world_state_repo_with_level_db
)