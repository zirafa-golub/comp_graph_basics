add_executable(ray_tracer)

file(GLOB ray_tracer_sources "src/*.cpp")
file(GLOB ray_tracer_includes "src/*.h")

target_sources(ray_tracer PRIVATE ${ray_tracer_sources} PUBLIC ${ray_tracer_includes})