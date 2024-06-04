add_library(rasterizer STATIC)

file(GLOB_RECURSE rasterizer_sources "src/*.cpp")
file(GLOB_RECURSE rasterizer_includes "inc/*.h")

target_sources(rasterizer PRIVATE ${rasterizer_sources} ${rasterizer_includes})
target_include_directories(rasterizer PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
target_link_libraries(rasterizer PUBLIC renderer core common mesh)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${rasterizer_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${rasterizer_includes})


add_executable(rasterizer_test)
file(GLOB_RECURSE rasterizer_test_sources "test/*.cpp")
file(GLOB_RECURSE rasterizer_test_includes "test/*.h")

target_sources(rasterizer_test PRIVATE ${rasterizer_test_sources} ${rasterizer_test_includes})
target_include_directories(rasterizer_test PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/test>")
target_link_libraries(rasterizer_test PRIVATE rasterizer test_utils gtest_main)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/test" FILES ${rasterizer_test_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/test" FILES ${rasterizer_test_includes})

add_test(NAME rasterizer_tests COMMAND rasterizer_test)