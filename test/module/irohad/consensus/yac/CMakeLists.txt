# Testing of transaction processor
addtest(cluster_order_test cluster_order_test.cpp)
target_link_libraries(cluster_order_test
    yac
    )

addtest(yac_cold_case_test yac_simple_cold_case_test.cpp)
target_link_libraries(yac_cold_case_test
    yac
    )

addtest(yac_sunny_day_test yac_sunny_day_test.cpp)
target_link_libraries(yac_sunny_day_test
    yac
    )

addtest(yac_storage_test yac_storage_test.cpp)
target_link_libraries(yac_storage_test
    yac
    model
    )