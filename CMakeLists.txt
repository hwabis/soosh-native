cmake_minimum_required(VERSION 3.24)

project(
  SooshNative
  LANGUAGES CXX
)

include(cmake/dependencies.cmake)

add_subdirectory(src)

include(CTest)
enable_testing()
add_subdirectory(tests)
