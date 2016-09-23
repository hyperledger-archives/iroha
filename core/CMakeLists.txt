set(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++1y")
SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -O0 -std=c++1y")
SET(CMAKE_SHARED_LINKER_FLAGS "-lpthread -lssl")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/peer)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(crypto)
add_subdirectory(domain)
#add_subdirectory(consensus)
add_subdirectory(peer)
add_subdirectory(util)
add_subdirectory(vendor)
add_subdirectory(smartContract)
#add_subdirectory(infra)
add_subdirectory(server)
