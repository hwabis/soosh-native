include(${CMAKE_SOURCE_DIR}/cmake/AddTestExecutable.cmake)

add_test_executable(soosh-client-tests soosh-client-lib
  test_client.cc
)
