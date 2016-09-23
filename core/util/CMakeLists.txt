

ADD_LIBRARY(logger  STATIC logger.cpp)
target_link_libraries(logger
  datetime
)

ADD_LIBRARY(datetime  STATIC time.cpp)

