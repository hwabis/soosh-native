include(${CMAKE_SOURCE_DIR}/cmake/SetTargetDefaults.cmake)

add_library(soosh-server-lib
  game_logic/scoring.cc
  game_logic/soosh_session.cc
  handlers/soosh_message_handler.cc
  networking/client_session.cc
  networking/server.cc
  utils/logger.cc
)

set_target_defaults(soosh-server-lib)

target_include_directories(soosh-server-lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  $<TARGET_PROPERTY:soosh-proto,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(soosh-server-lib PRIVATE
  Boost::asio
  soosh-proto
  soosh-proto-utils
)
