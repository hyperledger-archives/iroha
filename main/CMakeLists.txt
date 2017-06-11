core/connection/CMakeLists.txtadd_executable(iroha-main iroha-main.cpp)
target_link_libraries(iroha-main
    connection
)
