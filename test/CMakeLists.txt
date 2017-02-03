set(CMAKE_BUILD_TYPE Debug)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

add_subdirectory(crypto)
#add_subdirectory(smart_contract)
#add_subdirectory(infra/repository)
add_subdirectory(infra/service)
add_subdirectory(consensus)
add_subdirectory(vendor)
