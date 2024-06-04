#pragma once

#include "core/Mesh.h"
#include "core/Sphere.h"
#include "hit/MeshHitDetector.h"
#include "hit/SphereHitDetector.h"
#include "ray_tracer/RayTracerShaders.h"
#include "ShapeFactory.h"

#include <concepts>

namespace cg {
template <typename ShapeType>
    requires std::derived_from<ShapeType, Shape>
struct RayTracerShaderFactory {
    static std::unique_ptr<ShaderGroup> create()
        requires std::same_as<ShapeType, Sphere>
    {
        return std::make_unique<RayTracerShaders>(std::make_unique<SphereHitDetector>());
    }

    static std::unique_ptr<ShaderGroup> create()
        requires std::same_as<ShapeType, Mesh>
    {
        return std::make_unique<RayTracerShaders>(std::make_unique<MeshHitDetector>());
    }
};

static_assert(IsShaderGroupFactory<RayTracerShaderFactory>,
              "RayTracerShaderFactory does not support creating shaders for all of the required shapes.");
} // namespace cg
