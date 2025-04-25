include(${CMAKE_SOURCE_DIR}/cmake/SetTargetDefaults.cmake)

add_executable(soosh-client-app
  main.cc
)

set_target_defaults(soosh-client-app)

target_link_libraries(soosh-client-app PRIVATE
  soosh-client-lib
  CLI11::CLI11
)
