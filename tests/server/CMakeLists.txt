include(${CMAKE_SOURCE_DIR}/cmake/AddTestExecutable.cmake)

add_test_executable(soosh-server-tests soosh-server-lib
  test_game_session.cc
)
