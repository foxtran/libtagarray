cmake_minimum_required(VERSION 3.21)

function(generate_header LANGUAGE CONFIG_FILENAME HEADER_FILENAME)
  # check LANGUAGEs
  set(SUPPORTED_LANGUAGES C;Fortran)
  if(NOT ${LANGUAGE} IN_LIST SUPPORTED_LANGUAGES)
    message(FATAL_ERROR "Unsupported LANGUAGE: ${LANGUAGE}!")
  endif()
  # prolog (and epilog reading)
  file(READ ${CMAKE_SOURCE_DIR}/cmake/defines/${LANGUAGE}.head.in FILE_CONTEXT)
  file(READ ${CMAKE_SOURCE_DIR}/cmake/defines/${LANGUAGE}.tail.in FILE_CONTEXT_END)
  # defines
  file(STRINGS ${CONFIG_FILENAME} CSV)
  foreach(line ${CSV})
    string(REGEX REPLACE "^E.*" " " line ${line})
    if(${LANGUAGE} STREQUAL "C")
      string(REGEX REPLACE "^C " "// " line ${line})
      string(REGEX REPLACE " EQ " " " line ${line})
      string(REGEX REPLACE "^DI " "#define " line ${line})
      string(REGEX REPLACE "^DC " "#define " line ${line})
    elseif(${LANGUAGE} STREQUAL "Fortran")
      string(REGEX REPLACE "^C " "! " line ${line})
      string(REGEX REPLACE " EQ " " = " line ${line})
      string(REGEX REPLACE "^DI " "  integer(c_int32_t), parameter, public :: " line ${line})
      string(REGEX REPLACE "^DC " "  character(kind=TAGARRAY_CHAR, len=*), parameter, public :: " line ${line})
    else()
      message(FATAL_ERROR "Unsupported LANGUAGE: ${LANGUAGE}!")
    endif()
    string(CONCAT FILE_CONTEXT "${FILE_CONTEXT}" "${line}" "\n")
  endforeach()
  # epilog
  string(CONCAT FILE_CONTEXT "${FILE_CONTEXT}" "${FILE_CONTEXT_END}" "\n")
  # configure to file
  file(CONFIGURE OUTPUT ${HEADER_FILENAME}
     CONTENT ${FILE_CONTEXT}
     @ONLY)
endfunction()

load_cache(${CMAKE_CACHE_DIR} READ_WITH_PREFIX "" CMAKE_PROJECT_VERSION CMAKE_PROJECT_VERSION_MAJOR CMAKE_PROJECT_VERSION_MINOR CMAKE_PROJECT_VERSION_PATCH)

generate_header(${LANGUAGE} ${CONFIGFILE} ${HEADERFILE})
