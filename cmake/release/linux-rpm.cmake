find_program(RPMBUILD rpmbuild)
if(RPMBUILD STREQUAL "RPMBUILD-NOTFOUND")
  message(WARNING "linux-rpm can not be build without `rpmbuild`. Use -DNO_RPM to disable.")
else()
  # add deb to list of generators
  list(APPEND CPACK_GENERATOR RPM)

  get_current_architecture(arch)

  message(STATUS "[package-${arch}] linux-rpm standalone will be packaged. Use -DNO_RPM to disable.")

  SET(CPACK_RPM_PACKAGE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
  SET(CPACK_RPM_PACKAGE_REQUIRES "libstdc++5, libpq-dev, libtbb-dev, libboost-system-dev, libboost-filesystem-dev, libc-ares-dev")

endif()

