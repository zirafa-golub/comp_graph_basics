add_library(ray_tracer STATIC)

file(GLOB_RECURSE ray_tracer_sources "src/*.cpp")
file(GLOB_RECURSE ray_tracer_includes "inc/*.h")

target_sources(ray_tracer PRIVATE ${ray_tracer_sources} ${ray_tracer_includes})
target_include_directories(ray_tracer PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
target_link_libraries(ray_tracer PUBLIC screen core common)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${ray_tracer_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${ray_tracer_includes})
