add_executable(ray_tracer)

file(GLOB ray_tracer_sources "src/*.cpp")
file(GLOB ray_tracer_includes "inc/*.h")

target_sources(ray_tracer PRIVATE ${ray_tracer_sources} ${ray_tracer_includes})
target_include_directories(ray_tracer PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
target_link_libraries(ray_tracer PRIVATE core common asset SDL2::SDL2-static SDL2::SDL2main)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${ray_tracer_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${ray_tracer_includes})
