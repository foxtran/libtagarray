set(dir "${CMAKE_CURRENT_SOURCE_DIR}")

list(APPEND SOURCES_Python
  ${dir}/CXXAPI.cpp
)

set(SOURCES_Python ${SOURCES_Python} PARENT_SCOPE)
