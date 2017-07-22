# Testing of transaction processor
addtest(cluster_order_test cluster_order_test.cpp)
target_link_libraries(cluster_order_test
    yac
    )

addtest(yac_simple_case_test yac_simple_case_test.cpp)
target_link_libraries(yac_simple_case_test
    yac
    )