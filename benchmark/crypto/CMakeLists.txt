include_directories(
  ${PROJECT_SOURCE_DIR}/core
)

add_executable(hash_benchmark
  hash.cpp
)
target_link_libraries(hash_benchmark
  benchmark
  hash
)
