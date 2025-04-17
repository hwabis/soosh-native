include(${CMAKE_SOURCE_DIR}/cmake/SetTargetDefaults.cmake)

add_library(soosh-native
  main.cc
)

set_target_defaults(soosh-native)
