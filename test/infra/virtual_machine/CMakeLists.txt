set(CMAKE_BUILD_TYPE Debug)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++1y -Wall -fPIC -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/server")
  SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -g -std=c++1y -Wall -fPIC -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/server")
ELSE()
  SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++1y -Wall -fPIC -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/amd64")
  SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -g -std=c++1y -Wall -fPIC -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/amd64")
ENDIF()

include_directories(
    ${GTEST_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/core
    $ENV{JAVA_HOME}/include
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


add_executable(java_data_structure_test
    java_data_structure_test.cpp
)
add_dependencies(java_data_structure_test googletest)
target_link_libraries(java_data_structure_test
   ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread
)
target_link_libraries(java_data_structure_test
  virtual_machine_with_java
)
add_test(
    NAME java_data_structure_test
    COMMAND $<TARGET_FILE:java_data_structure_test>
)
