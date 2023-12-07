add_executable(test-utils-cpp test-utils.cpp)
target_link_libraries(test-utils-cpp ${PROJECT_NAME})
target_include_directories(test-utils-cpp PRIVATE "${CMAKE_BINARY_DIR}/include/tagarray")
add_test(NAME C++API::utils
         COMMAND test-utils-cpp)

add_executable(test-api-cpp test-api.cpp)
target_link_libraries(test-api-cpp ${PROJECT_NAME})
target_include_directories(test-api-cpp PRIVATE "${CMAKE_BINARY_DIR}/include/tagarray")
add_test(NAME C++API::api
         COMMAND test-api-cpp)
