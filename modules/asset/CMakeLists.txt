add_library(asset STATIC)

file(GLOB_RECURSE asset_sources "src/*.cpp")
file(GLOB_RECURSE asset_includes "inc/*.h")

target_sources(asset PRIVATE ${asset_sources} ${asset_includes})
target_include_directories(asset PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
target_link_libraries(asset PUBLIC core PRIVATE tinyobjloader)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${asset_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${asset_includes})