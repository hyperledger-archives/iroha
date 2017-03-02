IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++1y -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/server")
  SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++1y -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/server")

  include_directories(
          $ENV{JAVA_HOME}/include/darwin/
          $ENV{JAVA_HOME}/include/
  )
  link_directories(
          $ENV{JAVA_HOME}/jre/lib/server
          $ENV{JAVA_HOME}/jre/lib/amd64/server
  )
ELSE()
  SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++1y -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/amd64/server")
  SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++1y -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/amd64/server")

  include_directories(
    $ENV{JAVA_HOME}/include/linux
    $ENV{JAVA_HOME}/include
  )
  link_directories(
    $ENV{JAVA_HOME}/jre/lib/amd64/server/
  )
ENDIF()

include_directories(
  ${PROJECT_SOURCE_DIR}/core
)

###################################################
# java data structure
# 
# link
#   - create_objects_helper
###################################################
add_library(java_data_structure STATIC
  java_data_structure.cpp
)

target_link_libraries(java_data_structure
  create_objects_helper
  exception
)

###################################################
# java virtual machine
#
# link
#   - java_virtual_machine
#   - java_data_structure
###################################################
add_library(java_virtual_machine STATIC
  java_virtual_machine.cpp
)

target_link_libraries(java_virtual_machine
  $ENV{JAVA_HOME}/jre/lib/amd64/server/libjvm.so
  java_data_structure
  json
)  