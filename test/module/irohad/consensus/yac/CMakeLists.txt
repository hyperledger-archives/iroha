# Testing of transaction processor
addtest(cluster_order_test cluster_order_test.cpp)
target_link_libraries(cluster_order_test
    yac
)