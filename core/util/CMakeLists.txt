SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(JSON_PATH "${PROJECT_SOURCE_DIR}/core/vendor/json")

ADD_LIBRARY(datetime  STATIC datetime.cpp)

ADD_LIBRARY(logger  STATIC logger.cpp)
TARGET_LINK_LIBRARIES(logger
  datetime
)

ADD_LIBRARY(random              STATIC random.cpp)
ADD_LIBRARY(exception           STATIC exception.cpp)
ADD_LIBRARY(terminate           STATIC terminate.cpp)
