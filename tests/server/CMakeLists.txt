include(${CMAKE_SOURCE_DIR}/cmake/AddTestExecutable.cmake)

add_test_executable(soosh-native-server-tests soosh-native-server-lib
  test_server.cc
)
