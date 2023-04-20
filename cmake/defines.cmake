function(generate_header LANGUAGE CONFIGFILE HEADERFILE)
  add_custom_command(
    OUTPUT ${HEADERFILE}
    COMMAND ${CMAKE_COMMAND}
      -DCMAKE_CACHE_DIR=${CMAKE_BINARY_DIR}
      -DLANGUAGE=${LANGUAGE}
      -DCONFIGFILE=${CONFIGFILE}
      -DHEADERFILE=${HEADERFILE}
      -P ${PROJECT_SOURCE_DIR}/cmake/defines_generator.cmake
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    VERBATIM
    DEPENDS ${CONFIGFILE}
            ${CMAKE_SOURCE_DIR}/cmake/defines/${LANGUAGE}.head.in
            ${CMAKE_SOURCE_DIR}/cmake/defines/${LANGUAGE}.tail.in
  )
  add_custom_target(${PROJECT_NAME}_${LANGUAGE}_defines DEPENDS ${HEADERFILE})
endfunction()
