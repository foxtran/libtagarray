set(dir "${CMAKE_CURRENT_SOURCE_DIR}")

list(APPEND SOURCES_Fortran
  ${dir}/CAPI.f90
  ${dir}/container.f90
  ${dir}/Crecordinfo.f90
  ${dir}/recordinfo.f90
  ${dir}/tagarray.f90
  ${dir}/utils.F90
)

set(SOURCES_Fortran ${SOURCES_Fortran} PARENT_SCOPE)
