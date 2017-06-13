include(ExternalProject)
set(EP_PREFIX "${PROJECT_SOURCE_DIR}/external")
set_directory_properties(PROPERTIES
  EP_PREFIX ${EP_PREFIX}
)

# Project dependencies.
find_package(Threads REQUIRED)


###########################
#         keccak          #
###########################
find_package(LibXslt QUIET)
if (NOT LIBXSLT_XSLTPROC_EXECUTABLE)
  message(FATAL_ERROR "xsltproc not found")
endif ()

ExternalProject_Add(gvanas_keccak
  GIT_REPOSITORY    "https://github.com/gvanas/KeccakCodePackage.git"
  BUILD_IN_SOURCE   1
  BUILD_COMMAND     bash -c "CFLAGS='-fPIC -DKeccakP200_excluded -DKeccakP400_excluded -DKeccakP800_excluded'\
    $(MAKE) CC='${CMAKE_C_COMPILER}' generic64/libkeccak.a"
  CONFIGURE_COMMAND "" # remove configure step
  INSTALL_COMMAND   "" # remove install step
  TEST_COMMAND      "" # remove test step
  UPDATE_COMMAND    "" # remove update step
)
ExternalProject_Get_Property(gvanas_keccak source_dir)
set(keccak_SOURCE_DIR "${source_dir}")

# Hash internals for keccak
add_library(keccak STATIC IMPORTED)
file(MAKE_DIRECTORY ${keccak_SOURCE_DIR}/bin/generic64/libkeccak.a.headers)
set_target_properties(keccak PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${keccak_SOURCE_DIR}/bin/generic64/libkeccak.a.headers"
  IMPORTED_LOCATION "${keccak_SOURCE_DIR}/bin/generic64/libkeccak.a"
  )

add_dependencies(keccak gvanas_keccak)



############################
#         ed25519          #
############################
ExternalProject_Add(mizukisonoko_ed25519
  GIT_REPOSITORY    "https://github.com/MizukiSonoko/ed25519.git"
  BUILD_IN_SOURCE   1
  BUILD_COMMAND     $(MAKE)
  CONFIGURE_COMMAND "" # remove configure step
  INSTALL_COMMAND   "" # remove install step
  TEST_COMMAND      "" # remove test step
  UPDATE_COMMAND    "" # remove update step
  )
ExternalProject_Get_Property(mizukisonoko_ed25519 source_dir)
set(ed25519_SOURCE_DIR "${source_dir}")

add_library(ed25519 STATIC IMPORTED)
file(MAKE_DIRECTORY ${ed25519_SOURCE_DIR}/src)
set_target_properties(ed25519 PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${ed25519_SOURCE_DIR}/src
  IMPORTED_LOCATION ${ed25519_SOURCE_DIR}/lib/libed25519.a
  )
add_dependencies(ed25519 mizukisonoko_ed25519)



####################################
#         thread-pool-cpp          #
####################################
ExternalProject_Add(warchant_thread_pool
  GIT_REPOSITORY    "https://github.com/Warchant/thread-pool-cpp.git"
  BUILD_COMMAND     "" # remove build step, header only lib
  CONFIGURE_COMMAND "" # remove configure step
  INSTALL_COMMAND   "" # remove install step
  TEST_COMMAND      "" # remove test step
  UPDATE_COMMAND    "" # remove update step
  )
ExternalProject_Get_Property(warchant_thread_pool source_dir)
set(thread_pool_SOURCE_DIR "${source_dir}")

# since it is header only, we changed STATIC to INTERFACE below
add_library(thread_pool INTERFACE IMPORTED)
file(MAKE_DIRECTORY ${thread_pool_SOURCE_DIR}/thread_pool)
set_target_properties(thread_pool PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${thread_pool_SOURCE_DIR}/thread_pool
  )
add_dependencies(thread_pool warchant_thread_pool)


##########################
#         gtest          #
##########################
# testing is an option. Look at the main CMakeLists.txt for details.
if(TESTING)
  ExternalProject_Add(google_test
    GIT_REPOSITORY    "https://github.com/google/googletest.git"
    CMAKE_ARGS        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                      -Dgtest_force_shared_crt=ON
                      -Dgtest_disable_pthreads=OFF
                      -DBUILD_GTEST=ON
                      -DBUILD_GMOCK=OFF
    INSTALL_COMMAND   "" # remove install step
    UPDATE_COMMAND    "" # remove update step
    TEST_COMMAND      "" # remove test step
    )
  ExternalProject_Get_Property(google_test source_dir binary_dir)
  set(gtest_SOURCE_DIR ${source_dir})
  set(gtest_BINARY_DIR ${binary_dir})

  add_library(gtest STATIC IMPORTED)
  file(MAKE_DIRECTORY ${gtest_SOURCE_DIR}/googletest/include)

  set_target_properties(gtest
    PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${gtest_SOURCE_DIR}/googletest/include
    IMPORTED_LINK_INTERFACE_LIBRARIES "pthread;${gtest_BINARY_DIR}/googletest/libgtest_main.a"
    IMPORTED_LOCATION ${gtest_BINARY_DIR}/googletest/libgtest.a
  )
  add_dependencies(gtest google_test)
endif()



#############################
#         speedlog          #
#############################
ExternalProject_Add(gabime_spdlog
  GIT_REPOSITORY    "https://github.com/gabime/spdlog.git"
  GIT_TAG           "v0.13.0"
  CONFIGURE_COMMAND "" # remove configure step
  BUILD_COMMAND     "" # remove build step
  INSTALL_COMMAND   "" # remove install step
  TEST_COMMAND      "" # remove test step
  UPDATE_COMMAND    "" # remove update step
  )
ExternalProject_Get_Property(gabime_spdlog source_dir)
set(spdlog_INCLUDE_DIRS ${source_dir}/include)
file(MAKE_DIRECTORY ${spdlog_INCLUDE_DIRS})

add_library(spdlog INTERFACE IMPORTED)
set_target_properties(spdlog PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${spdlog_INCLUDE_DIRS}
  )

add_dependencies(spdlog gabime_spdlog)


#########################
#         protobuf      #
#########################
find_package(protobuf)

ExternalProject_Add(protobuf
        GIT_REPOSITORY    "https://github.com/google/protobuf"
        GIT_TAG           "v3.3.0"
        BUILD_IN_SOURCE   1
        BUILD_COMMAND     bash -c "./autogen.sh; ./configure; make -j"
        CONFIGURE_COMMAND "" # remove configure step
        INSTALL_COMMAND   "" # remove install step
        TEST_COMMAND      "" # remove test step
        UPDATE_COMMAND    "" # remove update step
        )
ExternalProject_Get_Property(protobuf source_dir)
set(protobuf_SOURCE_DIR "${source_dir}")



#########################
#         grpc          #
#########################
find_package(grpc)

if (NOT grpc_FOUND)
  ExternalProject_Add(grpc_grpc
    GIT_REPOSITORY "https://github.com/grpc/grpc.git"
    GIT_TAG           "v1.3.0"
    BUILD_IN_SOURCE   1
    BUILD_COMMAND     $(MAKE)
    CONFIGURE_COMMAND "" # remove configure step
    INSTALL_COMMAND   "" # remove install step
    TEST_COMMAND      "" # remove test step
    UPDATE_COMMAND    "" # remove update step
    )
  ExternalProject_Get_Property(grpc_grpc source_dir)
  set(grpc_INCLUDE_DIR   ${source_dir}/include/grpc)
  set(grpcpp_INCLUDE_DIR ${source_dir}/include/grpc++)
  set(grpc_LIB           ${source_dir}/libs/opt/libgrpc.so)
  set(grpcpp_LIB         ${source_dir}/libs/opt/libgrpc++.so)
  set(gpr_LIB            ${source_dir}/libs/opt/gpr.so)
endif ()

# libgrpc
add_library(grpc SHARED IMPORTED)
set_target_properties(grpc PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${grpc_INCLUDE_DIR}
  IMPORTED_LOCATION ${grpc_LIB}
  IMPORTED_LINK_INTERFACE_LANGUAGES "C"
)

# libgpr
add_library(gpr SHARED IMPORTED)
set_target_properties(gpr PROPERTIES
  IMPORTED_LOCATION ${gpr_LIB}
  IMPORTED_LINK_INTERFACE_LANGUAGES "C"
)

# libgrpc++
add_library(grpc++ SHARED IMPORTED)
set_target_properties(grpc++ PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${grpcpp_INCLUDE_DIR}
  IMPORTED_LOCATION ${grpcpp_LIB}
  IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
)

if(NOT grpc_FOUND)
  add_dependencies(grpc grpc_grpc)
endif()

