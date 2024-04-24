add_library(test_utils STATIC)

file(GLOB_RECURSE test_utils_sources "src/*.cpp")
file(GLOB_RECURSE test_utils_includes "inc/*.h")

target_sources(test_utils PRIVATE ${test_utils_sources} ${test_utils_includes})
target_include_directories(test_utils PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
target_link_libraries(test_utils PUBLIC glm::glm gtest)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${test_utils_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${test_utils_includes})