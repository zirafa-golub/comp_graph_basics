add_executable(renderer_app)

file(GLOB renderer_app_sources "src/*.cpp")
file(GLOB renderer_app_includes "inc/*.h")

target_sources(renderer_app PRIVATE ${renderer_app_sources} ${renderer_app_includes})
target_include_directories(renderer_app PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
target_link_libraries(renderer_app PRIVATE ray_tracer core common asset screen SDL2::SDL2-static SDL2::SDL2main)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${renderer_app_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${renderer_app_includes})
