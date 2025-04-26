add_library(soosh-proto-utils INTERFACE)

target_include_directories(soosh-proto-utils INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(soosh-proto-utils INTERFACE
  soosh-proto
  protobuf::libprotobuf
  Boost::asio
)
