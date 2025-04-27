include(${CMAKE_SOURCE_DIR}/cmake/SetTargetDefaults.cmake)

add_library(soosh-server-lib
  game_logic/game_session.cc
  networking/server_session.cc
  networking/server.cc
  game_message_handler.cc
)

set_target_defaults(soosh-server-lib)

target_include_directories(soosh-server-lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(soosh-server-lib PRIVATE
  Boost::asio
  soosh-proto
  soosh-proto-utils
)
