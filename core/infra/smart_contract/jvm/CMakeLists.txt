
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++11 -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/server")
  SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++11 -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/server")
ELSE()
  SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -std=c++11 -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/amd64/server")
  SET(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wextra -DDEBUG -O0 -std=c++11 -Wl,-rpath $ENV{JAVA_HOME}/jre/lib/amd64/server")
ENDIF()

include_directories($ENV{JAVA_HOME}/include)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  include_directories($ENV{JAVA_HOME}/include/darwin)
  link_directories($ENV{JAVA_HOME}/jre/lib/server)
ELSE()
  include_directories($ENV{JAVA_HOME}/include/linux)
  link_directories($ENV{JAVA_HOME}/jre/lib/amd64/server)
ENDIF()

ADD_LIBRARY(smart_contract STATIC java_virtual_machine.cpp)
target_link_libraries(smart_contract
  jvm
)  

