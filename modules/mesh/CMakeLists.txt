add_library(mesh STATIC)

file(GLOB_RECURSE mesh_sources "src/*.cpp")
file(GLOB_RECURSE mesh_includes "inc/*.h")

target_sources(mesh PRIVATE ${mesh_sources} ${mesh_includes})
target_include_directories(mesh PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
target_link_libraries(mesh PUBLIC core PRIVATE tinyobjloader)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${mesh_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${mesh_includes})

add_executable(mesh_test)
file(GLOB_RECURSE mesh_test_sources "test/*.cpp")

target_sources(mesh_test PRIVATE ${mesh_test_sources})
target_link_libraries(mesh_test PRIVATE mesh test_utils gtest_main)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/test" FILES ${mesh_test_sources})

add_test(NAME mesh_tests COMMAND mesh_test)