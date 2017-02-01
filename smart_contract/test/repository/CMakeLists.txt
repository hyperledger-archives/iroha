SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(
  /usr/lib/jvm/java-1.8.0-openjdk-amd64/include
  /usr/lib/jvm/java-1.8.0-openjdk-amd64/include/linux
  ${JAVA_HOME}/include
  ${JAVA_HOME}/include/linux
)

link_directories(${PROJECT_BINARY_DIR}/lib)

ADD_LIBRARY(DomainRepository SHARED
  domain_repository_jni.cpp
)
target_link_libraries(DomainRepository
  domain_repository
  java_virtual_machine
  event_with_grpc
)