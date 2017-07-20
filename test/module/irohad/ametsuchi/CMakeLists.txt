set(CMAKE_BUILD_TYPE Debug)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test_bin)

addtest(block_serializer_test block_serializer_test.cpp)
target_link_libraries(block_serializer_test
    model
    ametsuchi
    )

addtest(ametsuchi_test ametsuchi_test.cpp)
target_link_libraries(ametsuchi_test
    ametsuchi
    model
    )

addtest(flat_file_test flat_file_test.cpp)
target_link_libraries(flat_file_test
    ametsuchi
    )

#add_subdirectory(ametsuchi)

#
#
#addtest(block_index_test index/block_index_test.cpp)
#target_link_libraries(block_index_test
#        index
#    )
#
#addtest(tx_index_test index/tx_index_test.cpp)
#target_link_libraries(tx_index_test
#        index
#    )
#
#addtest(wsv_test wsv/wsv_test.cpp)
#target_link_libraries(wsv_test
#        wsv
#    )
#
