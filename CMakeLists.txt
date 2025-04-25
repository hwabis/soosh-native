cmake_minimum_required(VERSION 3.24)

project(
  SooshNative
  LANGUAGES CXX
)

find_package(CLI11 2.5 CONFIG REQUIRED)
find_package(Boost 1.88 CONFIG REQUIRED COMPONENTS asio)
find_package(Protobuf 5.29 CONFIG REQUIRED)
find_package(GTest 1.16 CONFIG REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)
