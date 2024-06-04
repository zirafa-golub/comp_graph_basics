#pragma once

#include "core/Mesh.h"
#include "core/Sphere.h"
#include "rasterizer/RasterizerShaders.h"
#include "shader/MeshShapeShader.h"
#include "shader/SphereShapeShader.h"
#include "ShapeFactory.h"

#include <concepts>

namespace cg {
template <typename ShapeType>
    requires std::derived_from<ShapeType, Shape>
struct RasterizerShaderFactory {
    static std::unique_ptr<ShaderGroup> create()
        requires std::same_as<ShapeType, Sphere>
    {
        return std::make_unique<RasterizerShaders>(std::make_unique<SphereShapeShader>(8, 16));
    }

    static std::unique_ptr<ShaderGroup> create()
        requires std::same_as<ShapeType, Mesh>
    {
        return std::make_unique<RasterizerShaders>(std::make_unique<MeshShapeShader>());
    }
};

static_assert(IsShaderGroupFactory<RasterizerShaderFactory>,
              "RasterizerShaderFactory does not support creating shaders for all of the required shapes.");
} // namespace cg
