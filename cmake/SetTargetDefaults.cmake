function(set_target_defaults target)
  set_target_properties(${target} PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
  )

  if (MSVC)
    target_compile_options(${target} PRIVATE
      /std:c++20
      /W4
      /analyze
      /external:anglebrackets
      /external:W0
      /EHsc

      /wd26440 # Do not litter noexcept everywhere
    )
    # todo: any way to enable include cleanup in visual studio from here?
  endif()
  # todo: g++/clang, cppcheck
endfunction()
