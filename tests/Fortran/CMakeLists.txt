add_executable(test-utils-f90 test-utils.F90)
target_link_libraries(test-utils-f90 ${PROJECT_NAME}_f)
target_include_directories(test-utils-f90 PRIVATE "${CMAKE_BINARY_DIR}/include/tagarray")
target_compile_options(test-utils-f90 PRIVATE $<$<COMPILE_LANGUAGE:Fortran>:-ffree-line-length-none>)
add_test(NAME FortranAPI::utils
         COMMAND test-utils-f90)
add_executable(test-recordinfo-f90 test-recordinfo.F90)
target_link_libraries(test-recordinfo-f90 ${PROJECT_NAME}_f)
target_include_directories(test-recordinfo-f90 PRIVATE "${CMAKE_BINARY_DIR}/include/tagarray")
target_compile_options(test-recordinfo-f90 PRIVATE $<$<COMPILE_LANGUAGE:Fortran>:-ffree-line-length-none>)
add_test(NAME FortranAPI::RecordInfo
         COMMAND test-recordinfo-f90)
add_executable(test-record-f90 test-record.F90)
target_link_libraries(test-record-f90 ${PROJECT_NAME}_f)
target_include_directories(test-record-f90 PRIVATE "${CMAKE_BINARY_DIR}/include/tagarray")
target_compile_options(test-record-f90 PRIVATE $<$<COMPILE_LANGUAGE:Fortran>:-ffree-line-length-none>)
add_test(NAME FortranAPI::Record
         COMMAND test-record-f90)
add_executable(test-container-f90 test-container.F90)
target_link_libraries(test-container-f90 ${PROJECT_NAME}_f)
target_include_directories(test-container-f90 PRIVATE "${CMAKE_BINARY_DIR}/include/tagarray")
target_compile_options(test-container-f90 PRIVATE $<$<COMPILE_LANGUAGE:Fortran>:-ffree-line-length-none>)
add_test(NAME FortranAPI::Container
         COMMAND test-container-f90)
