cmake_minimum_required(VERSION 3.20)

project(CompGraphBasics)

add_subdirectory("third_party")

include(CTest)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W3 /MP)
else()
  add_compile_options(-Wall)
endif()

add_subdirectory("modules/common")
add_subdirectory("modules/task")
add_subdirectory("modules/test_utils")
add_subdirectory("modules/core")
add_subdirectory("modules/mesh")
add_subdirectory("modules/renderer")
add_subdirectory("modules/ray_tracer")
add_subdirectory("modules/rasterizer")
add_subdirectory("modules/renderer_app")
