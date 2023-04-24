set(dir "${CMAKE_CURRENT_SOURCE_DIR}")

list(APPEND SOURCES
  ${dir}/C_API.cpp
  ${dir}/Container.cpp
  ${dir}/Record.cpp
)

add_subdirectory(API)

set(SOURCES ${SOURCES} PARENT_SCOPE)

if(ENABLE_FORTRAN)
  set(SOURCES_Fortran ${SOURCES_Fortran} PARENT_SCOPE)
endif()
