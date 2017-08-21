add_library(grpc UNKNOWN IMPORTED)
add_library(grpc++ UNKNOWN IMPORTED)
add_library(grpc++_reflection UNKNOWN IMPORTED)
add_executable(grpc_cpp_plugin IMPORTED)

find_path(grpc_INCLUDE_DIR grpc/grpc.h)
mark_as_advanced(grpc_INCLUDE_DIR)

find_library(grpc_LIBRARY grpc)
mark_as_advanced(grpc_LIBRARY)

find_library(grpc_grpc++_LIBRARY grpc++)
mark_as_advanced(grpc_grpc++_LIBRARY)

find_library(grpc_grpc++_reflection_LIBRARY grpc++_reflection)
mark_as_advanced(grpc_grpc++_reflection_LIBRARY)

find_program(grpc_CPP_PLUGIN grpc_cpp_plugin)
mark_as_advanced(grpc_CPP_PLUGIN)

find_package(PackageHandleStandardArgs REQUIRED)
find_package_handle_standard_args(grpc DEFAULT_MSG
    grpc_LIBRARY
    grpc_INCLUDE_DIR
    grpc_grpc++_reflection_LIBRARY
    grpc_CPP_PLUGIN
    )

if (NOT grpc_FOUND)
  externalproject_add(grpc_grpc
      GIT_REPOSITORY https://github.com/grpc/grpc
      GIT_TAG c80d3321d0f77bef8cfff8b32490a07c1e90a5ad
      BUILD_IN_SOURCE 1
      # 1. ${CMAKE_COMMAND} -E env - runs env command, which sets environment variables written as NAME=VALUE
      # 2. HAS_PKG_CONFIG=false - disable pkg-config so that it doesn't try to find protobuf.pc
      # 3. PROTOC=${protoc_EXECUTABLE}\ -I${protobuf_INCLUDE_DIR} - add path flag to protobuf executable, \ escapes space
      # 4. PATH=${protoc_EXECUTABLE_DIR}:$ENV{PATH} - append protoc folder to path
      # 5. LDFLAGS=-L${protobuf_LIBRARY_DIR} - add protobuf library dir for linker
      # 6. CFLAGS=-I${protobuf_INCLUDE_DIR} - add protobuf include dir for compiler
      # 7. CPPFLAGS=-I${protobuf_INCLUDE_DIR} - add protobuf include dir for preprocessor
      # 8. LD_LIBRARY_PATH=${protobuf_LIBRARY_DIR}:$ENV{LD_LIBRARY_PATH} - add protobuf include dir for library loader
      BUILD_COMMAND ${CMAKE_COMMAND} -E env HAS_PKG_CONFIG=false PROTOC=${protoc_EXECUTABLE}\ -I${protobuf_INCLUDE_DIR} LDFLAGS=-L${protobuf_LIBRARY_DIR} CFLAGS=-I${protobuf_INCLUDE_DIR} CPPFLAGS=-I${protobuf_INCLUDE_DIR} LD_LIBRARY_PATH=${protobuf_LIBRARY_DIR}:$ENV{LD_LIBRARY_PATH} $(MAKE)
      CONFIGURE_COMMAND "" # remove configure step
      INSTALL_COMMAND "" # remove install step
      TEST_COMMAND "" # remove test step
      UPDATE_COMMAND "" # remove update step
      )
  externalproject_get_property(grpc_grpc source_dir)
  set(grpc_INCLUDE_DIR ${source_dir}/include)
  set(grpc_LIBRARY ${source_dir}/libs/opt/libgrpc.so)
  set(grpc_grpc++_LIBRARY ${source_dir}/libs/opt/libgrpc++.so)
  set(grpc_grpc++_reflection_LIBRARY ${source_dir}/libs/opt/libgrpc++_reflection.so)
  set(grpc_CPP_PLUGIN ${source_dir}/bins/opt/grpc_cpp_plugin)
  file(MAKE_DIRECTORY ${grpc_INCLUDE_DIR})

  add_dependencies(grpc_grpc protobuf)
  add_dependencies(grpc grpc_grpc)
  add_dependencies(grpc++ grpc_grpc)
  add_dependencies(grpc++_reflection grpc_grpc)
  add_dependencies(grpc_cpp_plugin grpc_grpc protoc)
endif ()

set_target_properties(grpc PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${grpc_INCLUDE_DIR}
    INTERFACE_LINK_LIBRARIES "pthread;dl"
    IMPORTED_LOCATION ${grpc_LIBRARY}
    )

set_target_properties(grpc++ PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${grpc_INCLUDE_DIR}
    INTERFACE_LINK_LIBRARIES grpc
    IMPORTED_LOCATION ${grpc_grpc++_LIBRARY}
    )

set_target_properties(grpc++_reflection PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${grpc_INCLUDE_DIR}
    INTERFACE_LINK_LIBRARIES grpc++
    IMPORTED_LOCATION ${grpc_grpc++_reflection_LIBRARY}
    )

set_target_properties(grpc_cpp_plugin PROPERTIES
    IMPORTED_LOCATION ${grpc_CPP_PLUGIN}
    )
