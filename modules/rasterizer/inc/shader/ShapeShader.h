#pragma once

#include "core/MeshData.h"

namespace cg {
class ShaderGroup;
class Shape;

class ShapeShader {
public:
    virtual ~ShapeShader() = default;

    virtual MeshData generateMesh(const Shape& shape) const = 0;
};
} // namespace cg
