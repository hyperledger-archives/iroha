message(STATUS "[package] linux-deb standalone can be built")

# add deb to list of generators
list(APPEND CPACK_GENERATOR DEB)



set(CPACK_DEBIAN_PACKAGE_NAME          iroha  )
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE  amd64  )  # dpkg --print-architecture
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++5 (>= 1:3.3.6-28ubuntu1)")
