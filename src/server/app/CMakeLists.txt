include(${CMAKE_SOURCE_DIR}/cmake/SetTargetDefaults.cmake)

add_executable(soosh-native-server-app
  main.cc
)

set_target_defaults(soosh-native-server-app)

target_link_libraries(soosh-native-server-app PRIVATE
  soosh-native-server-lib
  CLI11::CLI11
)
