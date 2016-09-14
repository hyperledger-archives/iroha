set(CMAKE_BUILD_TYPE Debug)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++11 -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/server")
  SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++11 -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/server")
ELSE()
  SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++11 -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/amd64/server")
  SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++11 -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/amd64/server")
ENDIF()

SET(CMAKE_SHARED_LINKER_FLAGS "-ljvm")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/peer)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(KECCAK_PATH "../vendor/KeccakCodePackage")

include_directories($ENV{JAVA_HOME}/include)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  include_directories($ENV{JAVA_HOME}/include/darwin)
ENDIF()

link_directories($ENV{JAVA_HOME}/jre/lib/server)

ADD_LIBRARY(smartContract STATIC javaVM.cpp)

