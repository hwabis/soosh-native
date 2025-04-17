include(${CMAKE_SOURCE_DIR}/cmake/SetTargetDefaults.cmake)

add_executable(soosh-native-client-app
  main.cc
)

set_target_defaults(soosh-native-client-app)

target_link_libraries(soosh-native-client-app PRIVATE
  soosh-native-client-lib
)
