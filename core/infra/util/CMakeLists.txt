SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(MESSAGE_PACK_PATH ${PROJECT_SOURCE_DIR}/core/vendor/msgpack-c)

include_directories(
    ${MESSAGE_PACK_PATH}/include
)

link_directories(
  ${MESSAGE_PACK_PATH}
)

ADD_LIBRARY(convert_string_with_message_pack STATIC convert_string_with_message_pack.cpp)
target_link_libraries(convert_string_with_message_pack
  msgpackc
  exception
)  
