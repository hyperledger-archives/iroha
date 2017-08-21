add_library(uvw INTERFACE IMPORTED)

find_path(uvw_INCLUDE_DIR uvw.hpp)
mark_as_advanced(uvw_INCLUDE_DIR)

find_package(PackageHandleStandardArgs REQUIRED)
find_package_handle_standard_args(uvw DEFAULT_MSG
    uvw_INCLUDE_DIR
    )

if (NOT uvw_FOUND)
  externalproject_add(skypjack_uvw
      GIT_REPOSITORY https://github.com/skypjack/uvw
      GIT_TAG 00de1f1110ce4a9803a85a214af5326529f11312
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND "" # remove install step
      TEST_COMMAND "" # remove test step
      UPDATE_COMMAND "" # remove update step
      )
  externalproject_get_property(skypjack_uvw source_dir)
  set(uvw_INCLUDE_DIR ${source_dir}/src)
  file(MAKE_DIRECTORY ${uvw_INCLUDE_DIR})

  add_dependencies(skypjack_uvw uv)
  add_dependencies(uvw skypjack_uvw)
endif ()


set_target_properties(uvw PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${uvw_INCLUDE_DIR};${uv_INCLUDE_DIR}"
    INTERFACE_LINK_LIBRARIES "uv;pthread"
    )
