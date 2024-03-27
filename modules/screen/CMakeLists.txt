add_library(screen STATIC)

file(GLOB screen_sources "src/*.cpp")
file(GLOB screen_includes "inc/*.h")

target_sources(screen PRIVATE ${screen_sources} ${screen_includes})
target_include_directories(screen PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>")
target_link_libraries(screen PRIVATE core common PUBLIC SDL2::SDL2-static)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${screen_sources})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/inc" FILES ${screen_includes})
