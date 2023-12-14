add_library(common STATIC)

file(GLOB_RECURSE common_sources "src/*.cpp")
file(GLOB_RECURSE common_includes "inc/*.h")

target_sources(common PRIVATE ${common_sources} ${common_includes})
target_include_directories(common PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${common_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${common_includes})

add_executable(common_test)
file(GLOB_RECURSE common_test_sources "test/*.cpp")

target_sources(common_test PRIVATE ${common_test_sources})
target_link_libraries(common_test PRIVATE common gtest_main)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/test" FILES ${common_test_sources})

add_test(NAME common_tests COMMAND common_test)