cmake_minimum_required(VERSION 2.8)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(
  /usr/lib/jvm/java-1.8.0-openjdk-amd64/include
  /usr/lib/jvm/java-1.8.0-openjdk-amd64/include/linux
  ${JAVA_HOME}/include
  ${JAVA_HOME}/include/linux
  ${PROJECT_SOURCE_DIR}/core
  ${LEVELDB_PATH}/include
)

link_directories(
  ${PROJECT_BINARY_DIR}/lib
  ${PROJECT_SOURCE_DIR}/core/infra/crypto
  ${LEVELDB_PATH}/out-shared
  ${LEVELDB_PATH}/out-static
)

add_library(Repository SHARED
  repository_jni.cpp
)

target_link_libraries(Repository
  leveldb
  domain_repository
  virtual_machine_with_java
  event_with_grpc
  create_objects_helper
)

add_custom_command(TARGET Repository
  POST_BUILD
  COMMAND /bin/sh ${PROJECT_SOURCE_DIR}/smart_contract/compile_test.sh
)
