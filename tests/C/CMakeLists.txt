add_executable(test-api-c test-api.c)
target_link_libraries(test-api-c ${PROJECT_NAME})
target_include_directories(test-api-c PRIVATE "${CMAKE_BINARY_DIR}/include/tagarray")
add_test(NAME CAPI::api
         COMMAND test-api-c)
