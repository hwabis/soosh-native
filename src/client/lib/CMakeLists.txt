include(${CMAKE_SOURCE_DIR}/cmake/SetTargetDefaults.cmake)

add_library(soosh-native-client-lib
  client.cc
)

set_target_defaults(soosh-native-client-lib)

target_include_directories(soosh-native-client-lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
