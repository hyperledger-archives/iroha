SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# make config
add_executable(make_sumeragi make_sumeragi )
target_link_libraries(make_sumeragi
    base64
    signature
    json
)
