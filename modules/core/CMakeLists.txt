add_library(core STATIC)

file(GLOB_RECURSE core_sources "src/*.cpp")
file(GLOB_RECURSE core_includes "inc/*.h")

target_sources(core PRIVATE ${core_sources} ${core_includes})
target_include_directories(core PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
target_link_libraries(core PUBLIC common glm::glm)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${core_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${core_includes})


add_library(core_test_utils STATIC)
file(GLOB_RECURSE core_test_utils_sources "test/src/*.cpp")
file(GLOB_RECURSE core_test_utils_includes "test/inc/*.h")

target_sources(core_test_utils PRIVATE ${core_test_utils_sources} ${core_test_utils_includes})
target_include_directories(core_test_utils PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/test/inc>")
target_link_libraries(core_test_utils PUBLIC core PRIVATE gtest_main)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/test/src" FILES ${core_test_utils_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/test/inc" FILES ${core_test_utils_includes})


add_executable(core_test)
file(GLOB_RECURSE core_test_sources "test/tests/*.cpp")
file(GLOB_RECURSE core_test_includes "test/tests/*.h")

target_sources(core_test PRIVATE ${core_test_sources} ${core_test_includes})
target_include_directories(core_test PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/test/tests>")
target_link_libraries(core_test PRIVATE core test_utils core_test_utils gtest_main)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/test/tests" FILES ${core_test_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/test/tests" FILES ${core_test_includes})

add_test(NAME core_tests COMMAND core_test)