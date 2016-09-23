
# Datetime
ADD_LIBRARY(datetime  STATIC datetime.cpp)

# Logger
ADD_LIBRARY(logger  STATIC logger.cpp)
target_link_libraries(logger
  datetime
)

# Datetime
ADD_LIBRARY(exception  STATIC exception.cpp)

