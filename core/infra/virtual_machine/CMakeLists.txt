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

ADD_LIBRARY(virtual_machine_with_java STATIC virtual_machine_with_java.cpp)
target_link_libraries(virtual_machine_with_java
    java_data_structure
    java_virtual_machine
    logger
)
