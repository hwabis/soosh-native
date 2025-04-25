include(${CMAKE_SOURCE_DIR}/cmake/SetTargetDefaults.cmake)

add_executable(soosh-server-app
  main.cc
)

set_target_defaults(soosh-server-app)

target_link_libraries(soosh-server-app PRIVATE
  soosh-server-lib
  CLI11::CLI11
)
