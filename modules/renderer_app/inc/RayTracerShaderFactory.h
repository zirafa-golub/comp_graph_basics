#pragma once

#include "ShapeFactory.h"

namespace cg {
class RayTracerShaderFactory : public ShaderGroupFactory {
    std::unique_ptr<ShaderGroup> createSphereShaders() const override;
    std::unique_ptr<ShaderGroup> createMeshShaders() const override;
};
} // namespace cg
