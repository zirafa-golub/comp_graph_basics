add_library(tinyobjloader STATIC)

file(GLOB_RECURSE tinyobjloader_sources "*.cpp")
file(GLOB_RECURSE tinyobjloader_includes "*.h")

target_sources(tinyobjloader PRIVATE ${tinyobjloader_sources} ${tinyobjloader_includes})
target_include_directories(tinyobjloader PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${tinyobjloader_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${tinyobjloader_includes})