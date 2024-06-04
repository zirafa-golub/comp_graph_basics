add_library(renderer INTERFACE)

file(GLOB_RECURSE renderer_includes "inc/*.h")

target_sources(renderer INTERFACE ${renderer_includes})
target_include_directories(renderer INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
target_link_libraries(renderer INTERFACE core common)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${renderer_includes})
