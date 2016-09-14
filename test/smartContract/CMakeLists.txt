set(CMAKE_BUILD_TYPE Debug)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++11 -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/server")
  SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++11 -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/server")
ELSE()
  SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++11 -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/amd64")
  SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++11 -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/amd64")
ENDIF()


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

SET(VM_PATH  "${PROJECT_SOURCE_DIR}/core/smartContract")

set(GTEST_INCLUDE_DIR ${PROJECT_BINARY_DIR}/gtest/src/googletest/googletest/include)

SET(CMAKE_SHARED_LINKER_FLAGS "-ljvm")

include_directories(
  ${VM_PATH}
  ${GTEST_INCLUDE_DIR}
  $ENV{JAVA_HOME}/include
)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  include_directories($ENV{JAVA_HOME}/include/darwin)
ENDIF()


IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  link_directories(
    ${PROJECT_BINARY_DIR}/lib
    $ENV{JAVA_HOME}/jre/lib/server
  )
ELSE()
  link_directories(
    ${PROJECT_BINARY_DIR}/lib
    $ENV{JAVA_HOME}/jre/lib/amd64/server
  )
ENDIF()

add_executable(javaVM-test javaVM_test.cpp)
target_link_libraries(javaVM-test smartContract jvm
  ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread)
add_test(
  NAME javaVM-test
  COMMAND $<TARGET_FILE:javaVM-test>
)


