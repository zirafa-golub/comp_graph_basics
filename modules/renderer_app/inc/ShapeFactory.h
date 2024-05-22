#pragma once

#include "core/Mesh.h"
#include "core/ShaderGroup.h"
#include "core/Shape.h"
#include "core/Sphere.h"

#include <concepts>

namespace cg {
struct ShaderGroupFactory {
    virtual ~ShaderGroupFactory() = default;

    virtual std::unique_ptr<ShaderGroup> createSphereShaders() const = 0;
    virtual std::unique_ptr<ShaderGroup> createMeshShaders() const = 0;
};

struct ShapeFactory {
    template <typename... ShapeArgs>
    static std::unique_ptr<Sphere> createSphere(const ShaderGroupFactory& shaderFactory, ShapeArgs&&... shapeArgs) {
        auto shape = std::make_unique<Sphere>(std::forward<ShapeArgs>(shapeArgs)...);
        shape->setShaderGroup(shaderFactory.createSphereShaders());
        return shape;
    }

    template <typename... ShapeArgs>
    static std::unique_ptr<Mesh> createMesh(const ShaderGroupFactory& shaderFactory, ShapeArgs&&... shapeArgs) {
        auto shape = std::make_unique<Mesh>(std::forward<ShapeArgs>(shapeArgs)...);
        shape->setShaderGroup(shaderFactory.createMeshShaders());
        return shape;
    }
};
} // namespace cg
