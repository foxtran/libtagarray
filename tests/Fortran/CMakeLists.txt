add_executable(test-utils-f90 test-utils.F90)
target_link_libraries(test-utils-f90 ${PROJECT_NAME}_f)
target_include_directories(test-utils-f90 PRIVATE "${CMAKE_BINARY_DIR}/include/tagarray")
target_compile_options(test-utils-f90 PRIVATE $<$<COMPILE_LANGUAGE:Fortran>:-ffree-line-length-none>)
add_test(NAME FortranAPI::utils
         COMMAND test-utils-f90)
