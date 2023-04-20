if(ENABLE_FORTRAN)
  add_subdirectory(Fortran)
  set(SOURCES_Fortran ${SOURCES_Fortran} PARENT_SCOPE)
endif()
