set(FLATBUF_SRC_DIR "${PROJECT_SOURCE_DIR}/core/infra/flatbuf")

file(MAKE_DIRECTORY ${FLATBUF_SRC_DIR})
execute_process(COMMAND bash "-c" "cd $IROHA_HOME/flatbuf; flatc --gen-mutable --cpp --grpc -o ${FLATBUF_SRC_DIR} *.fbs")
execute_process(COMMAND bash "-c" "cd $IROHA_HOME/flatbuf; flatc --gen-mutable --cpp -o ${FLATBUF_SRC_DIR} *.fbs")

add_library(endpoint_fbs STATIC
    ${FLATBUF_SRC_DIR}/endpoint.grpc.fb.cc
)

target_link_libraries(endpoint_fbs
    grpc++
    grpc++_reflection
    grpc
    pthread
    dl
)
