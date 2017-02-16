add_subdirectory(objects)
add_subdirectory(commands)

include_directories(
  ${PROJECT_SOURCE_DIR}/core
  ${PROJECT_SOURCE_DIR}/core/model/objects
  ${PROJECT_SOURCE_DIR}/core/model/commands
  "."
)
