# add deb to list of generators
list(APPEND CPACK_GENERATOR DEB)

execute_process(
    COMMAND dpkg --print-architecture
    OUTPUT_VARIABLE arch_raw
)

remove_line_terminators(${arch_raw} arch)

message(STATUS "[package-${arch}] linux-deb standalone can be built")

set(CPACK_DEBIAN_PACKAGE_NAME          iroha  )
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE  ${arch})  # dpkg --print-architecture
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++5, libpq-dev, libtbb-dev, libboost-system-dev, libboost-filesystem-dev, libc-ares-dev")
