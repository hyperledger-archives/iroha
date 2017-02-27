SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

set(CMAKE_BUILD_TYPE Debug)
SET(LEVELDB_PATH ${PROJECT_SOURCE_DIR}/core/vendor/leveldb)

include_directories(
    ${LEVELDB_PATH}/include
    ${PROJECT_SOURCE_DIR}/core
)
link_directories(
    ${PROJECT_BINARY_DIR}/lib
    ${LEVELDB_PATH}/out-static
)

add_executable(world_state_repository_with_leveldb_test
    world_state_repository_with_leveldb_test.cpp
)
target_link_libraries(world_state_repository_with_leveldb_test
    leveldb
    snappy
    world_state_repo_with_level_db
    gtest

add_test(
    NAME world_state_repository_with_leveldb_test
    COMMAND $<TARGET_FILE:world_state_repository_with_leveldb_test>
)
