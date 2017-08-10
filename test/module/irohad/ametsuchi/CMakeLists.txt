addtest(ametsuchi_test ametsuchi_test.cpp)
target_link_libraries(ametsuchi_test
    ametsuchi
    )

addtest(flat_file_test flat_file_test.cpp)
target_link_libraries(flat_file_test
    ametsuchi
    )

addtest(testing_storage_test testing_storage_test.cpp)
target_link_libraries(testing_storage_test
    ametsuchi
    test_block_generator
    )
