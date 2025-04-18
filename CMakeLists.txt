cmake_minimum_required(VERSION 3.24)

project(
  SooshNative
  LANGUAGES CXX
)

include(cmake/dependencies.cmake)

add_subdirectory(src)
add_subdirectory(tests)
