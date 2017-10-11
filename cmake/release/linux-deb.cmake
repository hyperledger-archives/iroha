# add deb to list of generators
list(APPEND CPACK_GENERATOR DEB)

get_current_architecture(arch)

message(STATUS "[package-${arch}] linux-deb standalone will be packaged. Use -DNO_DEB to disable.")

set(CPACK_DEBIAN_PACKAGE_NAME          iroha  )
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE  ${arch})  # dpkg --print-architecture
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++5, libpq-dev, libtbb-dev, libboost-system-dev, libboost-filesystem-dev, libc-ares-dev")
