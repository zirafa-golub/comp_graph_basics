# Introduction
This is my C++ project for learning computer graphics. Since I'm very much a beginner in this field, it currently has just a simple ray tracer renderer and a rasterizer renderer, both implemented **in software** (i.e. on the CPU).
While everything is pretty basic regarding features (e.g. shading is using the basic Blinn-Phong model, no textures etc.), the implemented graphics pipeline is complete - from vertices to pixels on screen.
Hopefully, improvements will come with time and as I get more involved in the field (e.g. new features, optimizations, GPU implementation, better code design etc.).

## Build
The project is using some C++23 features, so you'll need a compiler supporting them. There is no OS specific stuff in the code, but I've only tried building and running it on Windows using Visual Studio 2022.

The build is CMake based, so you should be good to go as long as you have CMake 3.20. CMake should pull in and build the necessary dependencies:
- OpenGL GLM library
- SDL (for app setup and drawing pixels on screen)
- GoogleTest
- tinyobjloader

## Running
`renderer_app` target is the main (command line) app. It composes a *very* simple scene and renders it using one of the renderers. Since I haven't really focused on quality of life features so far (e.g. there's no CLI at the moment, no scene loading from file etc.), all tweaks need to be compiled.
There are two renderers available at the moment:
- `RayTraceRenderer`, CPU based, distributed to multiple cores
- `RasterizerRenderer`, CPU based, comes in 2 flavors: single-threaded and multi-threaded (with the `Parallel` suffix)

If you want to switch the renderer being used, you can do that fairly easily by changing which one is used to call the `runApp` template function. Make sure you pass in the corresponding shader factory too (`RayTracerShaderFactory` or `RasterizerShaderFactory`), otherwise it'll crash.
