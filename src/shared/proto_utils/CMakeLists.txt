add_library(soosh-proto-utils
  proto_enum_conversions.cc
  proto_streams.cc
)

target_include_directories(soosh-proto-utils PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}/src/shared/proto
)

target_link_libraries(soosh-proto-utils PUBLIC
  soosh-proto
)

target_link_libraries(soosh-proto-utils PRIVATE
  # todo how do i get rid of all the msvc compiler warnings for these lol
  protobuf::libprotobuf 
  Boost::asio
)
