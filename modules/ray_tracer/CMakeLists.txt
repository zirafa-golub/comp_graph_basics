add_library(ray_tracer STATIC)

file(GLOB_RECURSE ray_tracer_sources "src/*.cpp")
file(GLOB_RECURSE ray_tracer_includes "inc/*.h")

target_sources(ray_tracer PRIVATE ${ray_tracer_sources} ${ray_tracer_includes})
target_include_directories(ray_tracer PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
target_link_libraries(ray_tracer PUBLIC renderer core common task)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${ray_tracer_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${ray_tracer_includes})

add_executable(ray_tracer_test)
file(GLOB_RECURSE ray_tracer_test_sources "test/*.cpp")

target_sources(ray_tracer_test PRIVATE ${ray_tracer_test_sources})
target_link_libraries(ray_tracer_test PRIVATE ray_tracer test_utils gtest_main)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/test" FILES ${ray_tracer_test_sources})

add_test(NAME ray_tracer_tests COMMAND ray_tracer_test)