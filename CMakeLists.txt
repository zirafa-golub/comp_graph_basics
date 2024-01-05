cmake_minimum_required(VERSION 3.20)

project(CompGraphBasics)

add_subdirectory("third_party")

include(CTest)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W3)
else()
  add_compile_options(-Wall)
endif()

add_subdirectory("modules/common")
add_subdirectory("modules/core")
add_subdirectory("modules/asset")
add_subdirectory("modules/ray_tracer")
