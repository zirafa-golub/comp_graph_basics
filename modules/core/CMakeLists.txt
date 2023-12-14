add_library(core STATIC)

file(GLOB_RECURSE core_sources "src/*.cpp")
file(GLOB_RECURSE core_includes "inc/*.h")

target_sources(core PRIVATE ${core_sources} ${core_includes})
target_include_directories(core PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
target_link_libraries(core PUBLIC common glm::glm)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${core_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${core_includes})


add_executable(core_test)
file(GLOB_RECURSE core_test_sources "test/*.cpp")

target_sources(core_test PRIVATE ${core_test_sources})
target_link_libraries(core_test PRIVATE core gtest_main)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/test" FILES ${core_test_sources})

add_test(NAME common_tests COMMAND core_test)