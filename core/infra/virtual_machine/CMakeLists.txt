add_subdirectory(jvm)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  include_directories(
    $ENV{JAVA_HOME}/include/darwin/
    $ENV{JAVA_HOME}/include/
  )
  link_directories(
    $ENV{JAVA_HOME}/jre/lib/server
    $ENV{JAVA_HOME}/jre/lib/amd64/server
  )
ELSE()
  include_directories(
    $ENV{JAVA_HOME}/include/linux
    $ENV{JAVA_HOME}/include
  )
  link_directories(
    $ENV{JAVA_HOME}/jre/lib/amd64/server/
  )
ENDIF()

########################################################
# virtual_machine_with_java
########################################################

# Libraries

add_library(virtual_machine_with_java STATIC
  virtual_machine_with_java.cpp
)

# link
target_link_libraries(virtual_machine_with_java
  java_virtual_machine
  logger
)