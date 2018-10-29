# Testing of transaction processor
addtest(consensus_status_processor_test
    consensus_status_processor_test.cpp
    )
target_link_libraries(consensus_status_processor_test
    processors
    shared_model_stateless_validation
    )

# Testing of query processor
addtest(query_processor_test query_processor_test.cpp)
target_link_libraries(query_processor_test
    processors
    shared_model_cryptography
    )

# Testing of mst notifier
addtest(mst_notifier_test mst_notifier_test.cpp)
target_link_libraries(mst_notifier_test
    mst_notificator
    processors
    shared_model_stateless_validation
    shared_model_proto_builders
    )
