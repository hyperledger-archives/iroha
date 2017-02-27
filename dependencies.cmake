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

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DKeccakP200_excluded -DKeccakP400_excluded -DKeccakP800_excluded ")
ExternalProject_Add(gvanas_keccak
  GIT_REPOSITORY    "https://github.com/gvanas/KeccakCodePackage.git"
  BUILD_IN_SOURCE   1
  BUILD_COMMAND     $(MAKE) generic64/libkeccak.a VERBOSE=1
  CONFIGURE_COMMAND "" # remove configure step
  INSTALL_COMMAND   "" # remove install step
  TEST_COMMAND      "" # remove test step
  UPDATE_COMMAND    "" # remove update step
  )
ExternalProject_Get_Property(gvanas_keccak source_dir)
set(keccak_SOURCE_DIR "${source_dir}")

# CFLAGS="-DKeccakP200_excluded -DKeccakP400_excluded -DKeccakP800_excluded"; 

# Hash internals for keccak
add_library(keccak STATIC IMPORTED)
file(MAKE_DIRECTORY ${keccak_SOURCE_DIR}/bin/generic64/libkeccak.a.headers)
set_target_properties(keccak PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${keccak_SOURCE_DIR}/bin/generic64/libkeccak.a.headers"
  IMPORTED_LOCATION "${keccak_SOURCE_DIR}/bin/generic64/libkeccak.a"
  )

#add_library(keccak STATIC 
#  ${keccak_SOURCE_DIR}/SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c
#)
#target_link_libraries(keccak 
#  keccak_internal
#)
#set_target_properties(keccak 
#  PROPERTIES 
#  COMPILE_FLAGS "-DKeccakP200_excluded -DKeccakP400_excluded -DKeccakP800_excluded -std=c++0x"
#)

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



#########################
#         json          #
#########################
ExternalProject_Add(nlohmann_json
  GIT_REPOSITORY    "https://github.com/nlohmann/json.git"
  BUILD_COMMAND     "" # remove build step, header only lib
  CONFIGURE_COMMAND "" # remove configure step
  INSTALL_COMMAND   "" # remove install step
  TEST_COMMAND      "" # remove test step
  UPDATE_COMMAND    "" # remove update step
  )
ExternalProject_Get_Property(nlohmann_json source_dir)
set(json_SOURCE_DIR "${source_dir}")

# since it is header only, we changed STATIC to INTERFACE below
add_library(json INTERFACE IMPORTED)
file(MAKE_DIRECTORY ${json_SOURCE_DIR}/src)
set_target_properties(json PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${json_SOURCE_DIR}/src
  )
add_dependencies(json nlohmann_json)



##########################
#         gtest          #
##########################
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