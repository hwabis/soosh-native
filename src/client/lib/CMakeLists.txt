include(${CMAKE_SOURCE_DIR}/cmake/SetTargetDefaults.cmake)

add_library(soosh-client-lib
  client_session.cc
  client_ui.cc
  client.cc
)

set_target_defaults(soosh-client-lib)

target_include_directories(soosh-client-lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(soosh-client-lib PRIVATE
  Boost::asio
  soosh-proto
  soosh-proto-utils
)
