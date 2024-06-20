add_library(task STATIC)

file(GLOB_RECURSE task_sources "src/*.cpp")
file(GLOB_RECURSE task_includes "inc/*.h")

target_sources(task PRIVATE ${task_sources} ${task_includes})
target_include_directories(task PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${task_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${task_includes})

add_executable(task_test)
file(GLOB_RECURSE task_test_sources "test/*.cpp")

target_sources(task_test PRIVATE ${task_test_sources})
target_link_libraries(task_test PRIVATE task gtest_main)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/test" FILES ${task_test_sources})

add_test(NAME task_tests COMMAND task_test)
set_tests_properties(task_tests PROPERTIES TIMEOUT 3)