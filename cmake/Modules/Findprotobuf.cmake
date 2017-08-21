add_library(protobuf UNKNOWN IMPORTED)
add_executable(protoc IMPORTED)

find_path(protobuf_INCLUDE_DIR google/protobuf/service.h)
mark_as_advanced(protobuf_INCLUDE_DIR)

find_library(protobuf_LIBRARY protobuf)
mark_as_advanced(protobuf_LIBRARY)

find_program(protoc_EXECUTABLE protoc)
mark_as_advanced(protoc_EXECUTABLE)

find_package(PackageHandleStandardArgs REQUIRED)
find_package_handle_standard_args(protobuf DEFAULT_MSG
    protobuf_INCLUDE_DIR
    protobuf_LIBRARY
    protoc_EXECUTABLE
    )

if (NOT protobuf_FOUND)
  externalproject_add(google_protobuf
      GIT_REPOSITORY https://github.com/google/protobuf
      GIT_TAG a6189acd18b00611c1dc7042299ad75486f08a1a
      CONFIGURE_COMMAND ./autogen.sh && ./configure
      BUILD_IN_SOURCE 1
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND ""
      TEST_COMMAND "" # remove test step
      UPDATE_COMMAND "" # remove update step
      )
  externalproject_get_property(google_protobuf source_dir)
  set(protobuf_INCLUDE_DIR ${source_dir}/src)
  set(protobuf_LIBRARY ${source_dir}/src/.libs/libprotobuf.a)
  set(protoc_EXECUTABLE ${source_dir}/src/protoc)
  file(MAKE_DIRECTORY ${protobuf_INCLUDE_DIR})

  add_dependencies(protoc google_protobuf)
  add_dependencies(protobuf google_protobuf protoc)
endif ()

get_filename_component(protobuf_LIBRARY_DIR ${protobuf_LIBRARY} DIRECTORY)
mark_as_advanced(protobuf_LIBRARY_DIR)

set_target_properties(protobuf PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${protobuf_INCLUDE_DIR}
    IMPORTED_LOCATION ${protobuf_LIBRARY}
    )

set_target_properties(protoc PROPERTIES
    IMPORTED_LOCATION ${protoc_EXECUTABLE}
    )
