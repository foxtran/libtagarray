#
# add FLAGS to TARGET with VISIBILITY
#
# TARGET     name of target
# VISIBILITY PUBLIC/INTERFACE/PRIVATE
# LANGUAGE   for which language the set of flags should be checked. Only C, C++, and Fortran are supported now
# FLAGS      (the last argument) a set of flags. For example, "-Wall;-Wextra" represents two flags `-Wall` and `-Wextra`
function(target_compile_check_add_flags TARGET VISIBILITY LANGUAGE) # FLAGS
  string(TOUPPER "${LANGUAGE}" _LANGUAGE)
  # compute number of flags; exit, if 0
  list(LENGTH ARGV args_N)
  math(EXPR flags_N "${args_N} - 3")
  if(flags_N EQUAL 0)
    message(WARNING "No flags were passed to `target_compile_check_add_flags`")
    return()
  endif()
  # fill flags with magic
  set(_FLAGS "" ${ARGN})

  # Check and enable flags' checking
  if(${_LANGUAGE} STREQUAL "C")
    include(CheckCCompilerFlag)
  elseif(${_LANGUAGE} STREQUAL "CXX")
    include(CheckCXXCompilerFlag)
  elseif(${_LANGUAGE} STREQUAL "FORTRAN")
    include(CheckFortranCompilerFlag)
  else()
    message(FATAL_ERROR "Unsupported LANGUAGE: ${LANGUAGE}!")
  endif()

  # check all flags
  foreach(FLAG ${_FLAGS})
    # create a name for flag for a resulting variable from check_{lang}_compiler_flag
    # otherwise, FLAG_AVAILABLE... will not be reset
    string(REGEX REPLACE "[^A-Za-z0-9_]" "" FLAG_SHORTNAME "${FLAG}")

    # check flag
    if(${_LANGUAGE} STREQUAL "C")
      check_c_compiler_flag("${FLAG}" FLAG_AVAILABLE_${_LANGUAGE}_${FLAG_SHORTNAME})
    elseif(${_LANGUAGE} STREQUAL "CXX")
      check_cxx_compiler_flag("${FLAG}" FLAG_AVAILABLE_${_LANGUAGE}_${FLAG_SHORTNAME})
    elseif(${_LANGUAGE} STREQUAL "FORTRAN")
      check_fortran_compiler_flag("${FLAG}" FLAG_AVAILABLE_${_LANGUAGE}_${FLAG_SHORTNAME})
    endif()

    # attach flag to target if possible
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
