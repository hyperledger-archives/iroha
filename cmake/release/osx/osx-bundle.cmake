#message(STATUS "standalone osx-bundle can be packaged")
#
## TODO (@warchant): not tested. should be tested on mac
#set(APPS ${CMAKE_BINARY_DIR}/bin/irohad)
#set(DIRS ${CMAKE_SOURCE_DIR})
#INSTALL(CODE "
#  include(BundleUtilities)
#  fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
#"
#    COMPONENT Runtime)
