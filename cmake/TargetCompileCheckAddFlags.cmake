function(target_compile_check_add_flags TARGET VISIBILITY LANGUAGE FLAGS)
  string(TOUPPER "${LANGUAGE}" _LANGUAGE)
  set(_FLAGS ${FLAGS} ${ARGN})
  include(CheckCCompilerFlag)
  include(CheckCXXCompilerFlag)
  include(CheckFortranCompilerFlag)
  foreach(FLAG ${_FLAGS})
    string(REGEX REPLACE "[^A-Za-z0-9_]" "" FLAG_SHORTNAME "${FLAG}")
    if(${_LANGUAGE} STREQUAL "C")
      check_c_compiler_flag("${FLAG}" FLAG_AVAILABLE_${_LANGUAGE}_${FLAG_SHORTNAME})
    elseif(${_LANGUAGE} STREQUAL "CXX")
      check_cxx_compiler_flag("${FLAG}" FLAG_AVAILABLE_${_LANGUAGE}_${FLAG_SHORTNAME})
    elseif(${_LANGUAGE} STREQUAL "FORTRAN")
      check_fortran_compiler_flag("${FLAG}" FLAG_AVAILABLE_${_LANGUAGE}_${FLAG_SHORTNAME})
    else()
      message(FATAL_ERROR "Unsupported LANGUAGE: ${LANGUAGE}!")
    endif()
    if(FLAG_AVAILABLE_${_LANGUAGE}_${FLAG_SHORTNAME})
      separate_arguments(FLAG UNIX_COMMAND "${FLAG}")
      target_compile_options(${TARGET} ${VISIBILITY}
        $<$<COMPILE_LANGUAGE:${LANGUAGE}>:${FLAG}>
      )
    else()
      message(STATUS "Unsupported flag for ${LANGUAGE}: `${FLAG}`")
    endif()
  endforeach()
endfunction()
