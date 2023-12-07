cmake_minimum_required(VERSION 3.25)

project(tagarray
  VERSION 0.0.6
  LANGUAGES C CXX)


option(ENABLE_FORTRAN    "Enable Fortran bindings" OFF)
option(ENABLE_PYTHON     "Enable Python bindings (requires BUILD_SHARED_LIBS=ON)" OFF)
option(BUILD_SHARED_LIBS "Build as shared library" OFF)
option(ENABLE_TESTING    "Enable tests"            OFF)


if(ENABLE_FORTRAN)
  enable_language(Fortran)
endif()

if(ENABLE_PYTHON)
  if(NOT ${BUILD_SHARED_LIBS})
    message(FATAL_ERROR "ENABLE_PYTHON ON requires BUILD_SHARED_LIBS ON because only shared libraries can be dynamically loaded")
  endif()
  find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
  find_package(pybind11 REQUIRED)
endif()


set(PN ${PROJECT_NAME})


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CheckIPOSupported)

check_ipo_supported(RESULT IPO_AVAILABLE)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(defines)
include(CompilerFlagsSets)
include(TargetCompileCheckAddFlags)


if(NOT DEFINED ${CMAKE_CXX_COMPILER_ID}_CXX_WARNINGS)
  set(${CMAKE_CXX_COMPILER_ID}_CXX_WARNINGS "")
  message(WARNING "${CMAKE_CXX_COMPILER_ID} may not work properly")
endif()
if(ENABLE_FORTRAN)
  if(NOT DEFINED ${CMAKE_Fortran_COMPILER_ID}_Fortran_WARNINGS)
    set(${CMAKE_Fortran_COMPILER_ID}_Fortran_WARNINGS "")
    message(WARNING "${CMAKE_Fortran_COMPILER_ID} may not work properly")
  endif()
endif()


generate_header("C" "${CMAKE_SOURCE_DIR}/defines.csv" "${CMAKE_BINARY_DIR}/include/${PN}/defines.h")
generate_header("CXX" "${CMAKE_SOURCE_DIR}/defines.csv" "${CMAKE_BINARY_DIR}/include/${PN}/defines.hpp")
if(ENABLE_FORTRAN)
  generate_header("Fortran" "${CMAKE_SOURCE_DIR}/defines.csv" "${CMAKE_BINARY_DIR}/defines.f90")
endif()


set(SOURCES)
if(ENABLE_FORTRAN)
  set(SOURCES_Fortran)
endif()
if(ENABLE_PYTHON)
  set(SOURCES_Python)
endif()

add_subdirectory(source)


add_library(${PN} ${SOURCES})
add_dependencies(${PN} ${PN}_C_defines)
add_dependencies(${PN} ${PN}_CXX_defines)
target_sources(${PN}
  PUBLIC
    FILE_SET HEADERS BASE_DIRS ${PROJECT_SOURCE_DIR}/include FILES include/${PN}.h
                                                                   include/${PN}.hpp
                                                                   include/${PN}/RecordInfo.h
                                                                   include/${PN}/Container.hpp
                                                                   include/${PN}/private/HRecord.hpp
    FILE_SET HEADERS BASE_DIRS ${CMAKE_BINARY_DIR}/include   FILES ${CMAKE_BINARY_DIR}/include/${PN}/defines.h
)
target_include_directories(${PN} PRIVATE "${CMAKE_SOURCE_DIR}/include" "${CMAKE_BINARY_DIR}/include")
target_compile_features(${PN} PUBLIC cxx_std_17)
target_compile_check_add_flags(${PN} PRIVATE CXX     ${${CMAKE_CXX_COMPILER_ID}_CXX_WARNINGS})
set_target_properties(${PN} PROPERTIES VERSION ${PROJECT_VERSION}
                                       SOVERSION 1)
if(IPO_AVAILABLE)
  set_target_properties(${PN} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
install(TARGETS ${PN}
        EXPORT ${PN}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


if(ENABLE_FORTRAN)
  add_library(${PN}_f ${SOURCES_Fortran} "${CMAKE_BINARY_DIR}/defines.f90")
  target_compile_features(${PN}_f PRIVATE cxx_std_17)
  add_dependencies(${PN}_f ${PN}_Fortran_defines)
  target_link_libraries(${PN}_f ${PN})
  target_sources(${PN}_f
    PUBLIC
      FILE_SET HEADERS BASE_DIRS ${PROJECT_SOURCE_DIR}/include FILES include/${PN}.fh
                                                                     include/${PN}/${PN}_v1.fh
                                                                     include/${PN}/${PN}_v2.fh
                                                                     include/${PN}/${PN}_v3.fh
#      FILE_SET HEADERS BASE_DIRS ${CMAKE_BINARY_DIR}/include   FILES ${CMAKE_BINARY_DIR}/include/${PN}/tagarray.mod
#                                                                     ${CMAKE_BINARY_DIR}/include/${PN}/tagarray_defines.mod
#                                                                     ${CMAKE_BINARY_DIR}/include/${PN}/tagarray_capi.mod
#                                                                     ${CMAKE_BINARY_DIR}/include/${PN}/tagarray_utils.mod
#                                                                     ${CMAKE_BINARY_DIR}/include/${PN}/tagarray_container.mod
#                                                                     ${CMAKE_BINARY_DIR}/include/${PN}/tagarray_record.mod
#                                                                     ${CMAKE_BINARY_DIR}/include/${PN}/tagarray_recordinfo.mod
  )
  set_target_properties(${PN}_f PROPERTIES Fortran_MODULE_DIRECTORY "${CMAKE_BINARY_DIR}/include/${PN}"
                                           VERSION ${PROJECT_VERSION}
                                           SOVERSION 1)
  if(IPO_AVAILABLE AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set_target_properties(${PN}_f PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
  endif()
  try_compile(assumed_size_polymorphic_objects
    SOURCE_FROM_CONTENT assumed_size_polymorphic_objects.f90 "program test\ncontains\n  subroutine UP(arr)\n    class(*) :: arr(*)\n  end subroutine UP\nend program test")
  if(assumed_size_polymorphic_objects)
    set(MAX_TA_FORTRAN_API_VERSION_AVAILABLE 2)
    target_compile_definitions(${PN}_f PUBLIC TA_FORTRAN_API_VERSION_AVAILABLE=2)
  endif()
  if("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${PN}_f PRIVATE
      $<$<COMPILE_LANGUAGE:Fortran>:-std=f2018>
    )
  elseif("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "Intel")
    target_compile_options(${PN}_f PRIVATE
      $<$<COMPILE_LANGUAGE:Fortran>:-stand f18>
    )
  elseif("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "IntelLLVM")
    target_compile_options(${PN}_f PRIVATE
      $<$<COMPILE_LANGUAGE:Fortran>:-stand f18>
    )
  else()
    message(FATAL_ERROR "Unknown Fortran compiler ID: ${CMAKE_Fortran_COMPILER_ID}")
  endif()
  target_compile_check_add_flags(${PN}_f PRIVATE CXX     ${${CMAKE_CXX_COMPILER_ID}_CXX_WARNINGS})
  target_compile_check_add_flags(${PN}_f PRIVATE Fortran ${${CMAKE_Fortran_COMPILER_ID}_Fortran_WARNINGS})
  install(TARGETS ${PN}_f
          EXPORT ${PN}Targets
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
          FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
  install(FILES ${CMAKE_BINARY_DIR}/include/${PN}/tagarray.mod
          FILES ${CMAKE_BINARY_DIR}/include/${PN}/tagarray_defines.mod
          FILES ${CMAKE_BINARY_DIR}/include/${PN}/tagarray_capi.mod
          FILES ${CMAKE_BINARY_DIR}/include/${PN}/tagarray_utils.mod
          FILES ${CMAKE_BINARY_DIR}/include/${PN}/tagarray_container.mod
          FILES ${CMAKE_BINARY_DIR}/include/${PN}/tagarray_record.mod
          FILES ${CMAKE_BINARY_DIR}/include/${PN}/tagarray_recordinfo.mod
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PN})
endif()


if(ENABLE_PYTHON)
  pybind11_add_module(${PN}_py MODULE ${SOURCES_Python})
  target_compile_features(${PN}_py PRIVATE cxx_std_17)
  target_link_libraries(${PN}_py PRIVATE ${PN})
  target_compile_check_add_flags(${PN}_py PRIVATE CXX ${${CMAKE_CXX_COMPILER_ID}_CXX_WARNINGS})
  set_target_properties(${PN}_py PROPERTIES OUTPUT_NAME ${PN})
  if(IPO_AVAILABLE)
    set_target_properties(${PN}_py PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
  endif()
#  install(TARGETS ${PN}_py
#          EXPORT ${PN}Targets
#          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#          INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#          FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#  )
endif()


install(EXPORT ${PN}Targets
    FILE ${PN}Targets.cmake
    NAMESPACE ${PN}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PN})

write_basic_package_version_file(
    "${PN}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PN}ConfigVersion.cmake" DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PN})


if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
