include(${CMAKE_SOURCE_DIR}/cmake/SetTargetDefaults.cmake)

function(add_test_executable target test_lib)
  add_executable(${target} ${ARGN})

  set_target_defaults(${target})

  target_link_libraries(${target} PRIVATE
    ${test_lib}
    GTest::gtest_main
  )

  # todo: figure out a better way to suppress all gtest warnings
  if (MSVC)
    target_compile_options(${target} PRIVATE
      /wd26439
      /wd26495 # https://github.com/Azure/azure-sdk-for-cpp/issues/106
      /wd6326 # https://github.com/google/googletest/issues/471
    )
  endif()
endfunction()
