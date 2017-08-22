add_library(uv UNKNOWN IMPORTED)

find_path(uv_INCLUDE_DIR uv.h)
mark_as_advanced(uv_INCLUDE_DIR)

find_library(uv_LIBRARY uv)
mark_as_advanced(uv_LIBRARY)

find_package(PackageHandleStandardArgs REQUIRED)
find_package_handle_standard_args(uv DEFAULT_MSG
    uv_INCLUDE_DIR
    uv_LIBRARY
    )

if (NOT uv_FOUND)
  externalproject_add(libuv_libuv
      GIT_REPOSITORY https://github.com/libuv/libuv
      GIT_TAG 2bb4b68758f07cd8617838e68c44c125bc567ba6
      CONFIGURE_COMMAND ./autogen.sh && ./configure
      BUILD_IN_SOURCE 1
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND "" # remove install step
      TEST_COMMAND "" # remove test step
      UPDATE_COMMAND "" # remove update step
      )
  externalproject_get_property(libuv_libuv source_dir)
  set(uv_INCLUDE_DIR ${source_dir}/include)
  set(uv_LIBRARY ${source_dir}/.libs/libuv.a)
  file(MAKE_DIRECTORY ${uv_INCLUDE_DIR})

  add_dependencies(uv libuv_libuv)
endif ()

set_target_properties(uv PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${uv_INCLUDE_DIR}
    IMPORTED_LOCATION ${uv_LIBRARY}
    INTERFACE_LINK_LIBRARIES "pthread"
    )
