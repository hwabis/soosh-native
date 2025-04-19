include(${CMAKE_SOURCE_DIR}/cmake/SetTargetDefaults.cmake)

add_library(soosh-native-server-lib
  server.cc
)

set_target_defaults(soosh-native-server-lib)

target_include_directories(soosh-native-server-lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(soosh-native-server-lib SYSTEM PRIVATE
  ${Boost_INCLUDE_DIRS}
)
