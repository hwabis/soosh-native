set(PROTO_FILES soosh.proto)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

add_library(soosh-proto STATIC ${PROTO_SRCS} ${PROTO_HDRS})

target_include_directories(soosh-proto PUBLIC
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(soosh-proto PUBLIC
  protobuf::libprotobuf
)
