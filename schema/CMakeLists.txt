message(STATUS "Schema is at: ${IROHA_SCHEMA_DIR}")

function(compile_fbs_to_cpp FBS)

endfunction()

compile_fbs_to_cpp(account.fbs)
compile_fbs_to_cpp(asset.fbs)
compile_fbs_to_cpp(commands.fbs)
compile_fbs_to_cpp(endpoint.fbs)
compile_fbs_to_cpp(main.fbs)
compile_fbs_to_cpp(primitives.fbs)
compile_fbs_to_cpp(transaction.fbs)

add_library(endpoint_fbs STATIC
    ${IROHA_SCHEMA_DIR}/endpoint.grpc.fb.cc
    )

target_link_libraries(endpoint_fbs
    grpc++
    grpc
    gpr
    pthread
    dl
    flatbuffers
    )
