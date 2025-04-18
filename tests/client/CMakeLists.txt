include(${CMAKE_SOURCE_DIR}/cmake/AddTestExecutable.cmake)

add_test_executable(soosh-native-client-tests soosh-native-client-lib
  test_client.cc
)
