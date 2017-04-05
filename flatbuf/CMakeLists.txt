set(FLATBUF_SRC_DIR "${PROJECT_SOURCE_DIR}/core/infra/flatbuf")

file(MAKE_DIRECTORY ${FLATBUF_SRC_DIR})
execute_process(COMMAND bash "-c" "cd $IROHA_HOME/flatbuf; flatc --cpp --grpc -o ${FLATBUF_SRC_DIR} *.fbs")

add_library(event_with_grpc STATIC
    ${FLATBUF_SRC_DIR}/api.grpc.fb.cc
)

target_link_libraries(event_with_grpc
    grpc++
    grpc++_reflection
    flatbuf
    pthread
    dl
    ed25519
    hash
)
