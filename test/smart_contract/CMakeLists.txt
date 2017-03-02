SET(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS "-g -std=c++1y -Wall -fPIC")
SET(ED25519_PATH "${PROJECT_SOURCE_DIR}/core/vendor/ed25519")
SET(LEVELDB_PATH ${PROJECT_SOURCE_DIR}/core/vendor/leveldb)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++1y -Wall -fPIC -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/server")
  SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -g -std=c++1y -Wall -fPIC -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/server")
ELSE()
  SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++1y -Wall -fPIC -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/amd64")
  SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -g -std=c++1y -Wall -fPIC -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/amd64")
ENDIF()


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

include_directories(
  ${GTEST_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/core
  $ENV{JAVA_HOME}/include
  ${LEVELDB_PATH}/include
  ${ED25519_PATH}/src
)

link_directories(
  ${ED25519_PATH}/lib
  ${LEVELDB_PATH}/out-static
  ${PROJECT_SOURCE_DIR}/core/infra/crypto
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  include_directories($ENV{JAVA_HOME}/include/darwin)
  link_directories(
    ${PROJECT_BINARY_DIR}/lib
    $ENV{JAVA_HOME}/jre/lib/server
  )
ELSE()
  include_directories($ENV{JAVA_HOME}/include/linux)
  link_directories(
    ${PROJECT_BINARY_DIR}/lib
    $ENV{JAVA_HOME}/jre/lib/amd64/server
  )
ENDIF()

########################################################################################
# Executable java_query_repo_invocation_test
########################################################################################
add_executable(java_query_repo_invocation_test java_query_repo_invocation_test.cpp)
add_dependencies(java_query_repo_invocation_test googletest)
target_link_libraries(java_query_repo_invocation_test
  Repository
  
  # LevelDB depends on snappy
  leveldb
  snappy
  
  virtual_machine_with_java
  world_state_repo_with_level_db
  consensus_event
  jvm
  ${GTEST_LIBS_DIR}/libgtest.a
  ${GTEST_LIBS_DIR}/libgtest_main.a
  pthread)
add_test(
  NAME java_query_repo_invocation_test
  COMMAND $<TARGET_FILE:java_query_repo_invocation_test>
)


########################################################################################
# Executable java_query_repo_account_test
########################################################################################
add_executable(java_query_repo_account_test java_query_repo_account_test.cpp)

add_dependencies(java_query_repo_account_test
  googletest
)

target_link_libraries(java_query_repo_account_test
  Repository
  
  # LevelDB depends on snappy
  leveldb
  snappy
  
  virtual_machine_with_java
  world_state_repo_with_level_db
  consensus_event
  jvm
  ${GTEST_LIBS_DIR}/libgtest.a
  ${GTEST_LIBS_DIR}/libgtest_main.a
  pthread
)
add_test(
  NAME java_query_repo_account_test
  COMMAND $<TARGET_FILE:java_query_repo_account_test>
)

########################################################################################
# Executable java_query_repo_asset_test
########################################################################################
add_executable(java_query_repo_asset_test java_query_repo_asset_test.cpp)

add_dependencies(java_query_repo_asset_test
  googletest
)

target_link_libraries(java_query_repo_asset_test
  Repository

  # LevelDB depends on snappy
  leveldb
  snappy

  virtual_machine_with_java
  world_state_repo_with_level_db
  consensus_event
  jvm
  ${GTEST_LIBS_DIR}/libgtest.a
  ${GTEST_LIBS_DIR}/libgtest_main.a
  pthread
)
add_test(
  NAME java_query_repo_asset_test
  COMMAND $<TARGET_FILE:java_query_repo_asset_test>
)

########################################################################################
# Executable java_query_repo_simple_asset_test
########################################################################################
add_executable(java_query_repo_simple_asset_test java_query_repo_simple_asset_test.cpp)

add_dependencies(java_query_repo_simple_asset_test
  googletest
)

target_link_libraries(java_query_repo_simple_asset_test
  Repository

  # LevelDB depends on snappy
  leveldb
  snappy
  
  virtual_machine_with_java
  world_state_repo_with_level_db
  consensus_event
  jvm
  ${GTEST_LIBS_DIR}/libgtest.a
  ${GTEST_LIBS_DIR}/libgtest_main.a
  pthread
)
add_test(
  NAME java_query_repo_simple_asset_test
  COMMAND $<TARGET_FILE:java_query_repo_simple_asset_test>
)

########################################################################################
# Executable java_query_repo_domain_test
########################################################################################
add_executable(java_query_repo_domain_test java_query_repo_domain_test.cpp)

add_dependencies(java_query_repo_domain_test
  googletest
)

target_link_libraries(java_query_repo_domain_test
  Repository

  # LevelDB depends on snappy
  leveldb
  snappy
  
  virtual_machine_with_java
  world_state_repo_with_level_db
  consensus_event
  jvm
  ${GTEST_LIBS_DIR}/libgtest.a
  ${GTEST_LIBS_DIR}/libgtest_main.a
  pthread
)
add_test(
  NAME java_query_repo_domain_test
  COMMAND $<TARGET_FILE:java_query_repo_domain_test>
)

########################################################################################
# Executable java_query_repo_peer_test
########################################################################################
add_executable(java_query_repo_peer_test java_query_repo_peer_test.cpp)

add_dependencies(java_query_repo_peer_test
  googletest
)

target_link_libraries(java_query_repo_peer_test
  Repository

  # LevelDB depends on snappy
  leveldb
  snappy
  
  virtual_machine_with_java
  world_state_repo_with_level_db
  consensus_event
  jvm
  ${GTEST_LIBS_DIR}/libgtest.a
  ${GTEST_LIBS_DIR}/libgtest_main.a
  pthread
)
add_test(
  NAME java_query_repo_peer_test
  COMMAND $<TARGET_FILE:java_query_repo_peer_test>
)