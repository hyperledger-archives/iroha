########################################################################################
# PeerService
########################################################################################
add_executable(peer_service_test peer_service_test.cpp)
target_link_libraries(peer_service_test
  gtest
  membership_service

)
add_test(
  NAME peer_service_test
  COMMAND $<TARGET_FILE:peer_service_test>
)

add_executable(peer_service_to_issue_transaction_test peer_service_to_issue_transaction_test.cpp)
target_link_libraries(peer_service_to_issue_transaction_test
  gtest
  endpoint_fbs
  membership_service
  flatbuffer_service
  connection_with_grpc_flatbuffer
)
add_test(
  NAME peer_service_to_issue_transaction_test
  COMMAND $<TARGET_FILE:peer_service_to_issue_transaction_test>
)

add_executable(synchornizer_connection_part_test synchornizer_connection_part_test.cpp)
target_link_libraries(synchornizer_connection_part_test
  gtest
  ametsuchi
  endpoint_fbs
  membership_service
  flatbuffer_service
  connection_with_grpc_flatbuffer
)
add_test(
  NAME synchornizer_connection_part_test
  COMMAND $<TARGET_FILE:peer_service_to_issue_transaction_test>
)