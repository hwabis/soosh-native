include(FetchContent)
include(${CMAKE_SOURCE_DIR}/cmake/AddTestExecutable.cmake)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.16.0
)

FetchContent_MakeAvailable(googletest)

add_test_executable(soosh-native-client-tests soosh-native-client-lib
  test_henlo.cc
)

add_test(NAME AdderTest COMMAND soosh-native-client-tests)
