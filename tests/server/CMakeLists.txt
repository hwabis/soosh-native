include(${CMAKE_SOURCE_DIR}/cmake/AddTestExecutable.cmake)

add_test_executable(soosh-server-tests soosh-server-lib
  game_logic/test_join_leave.cc
  game_logic/test_play_logic.cc
  game_logic/test_scoring.cc
)
