INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_GENERATOR TGZ)

set(CPACK_PACKAGE_NAME "iroha")
set(CPACK_PACKAGE_VENDOR "Soramitsu LLC")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Iroha - simple decentralized blockchain")

SET(CPACK_PACKAGE_VENDOR              "Soramitsu LLC")
SET(CPACK_RESOURCE_FILE_LICENSE       "${CMAKE_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_CONTACT             "Bogdan Vaneev <bogdan@soramitsu.co.jp>")

SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "95")
SET(CPACK_PACKAGE_VERSION_PATCH "0")

SET(CPACK_STRIP_FILES TRUE)

set(CPACK_COMPONENTS_ALL irohad iroha-cli)

if(WIN32)
  # cmake is running on windows
elseif (APPLE)
  # cmake is running on mac os
  include(cmake/release/osx-bundle.cmake)
elseif(UNIX)
  # cmake is running on unix
  include(cmake/release/linux-deb.cmake)
endif()

INCLUDE(CPack)
