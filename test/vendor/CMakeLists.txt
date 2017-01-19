SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)
SET(TP_DIR "${PROJECT_SOURCE_DIR}/core/vendor/thread_pool_cpp")
SET(_DIR "${PROJECT_SOURCE_DIR}/core/vendor/json")
SET(GTEST_DIR "${GTEST_LIBS_DIR}")

include_directories(
  ${TP_DIR}
  ${GTEST_INCLUDE_DIR}
)

add_executable(thread_pool_test
  thread_pool_test.cpp
)

target_link_libraries(thread_pool_test
  pthread
  ${GTEST_LIBS_DIR}/libgtest.a 
  ${GTEST_LIBS_DIR}/libgtest_main.a 
)

add_test(
  NAME thread_pool_test
  COMMAND $<TARGET_FILE:thread_pool_test>
)

# json test
add_executable(json_test
    json_test.cpp
)
target_link_libraries(json_test
   ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a pthread
)
add_test(
  NAME json_test
  COMMAND $<TARGET_FILE:json_test>
)
