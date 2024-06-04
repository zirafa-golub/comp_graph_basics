#pragma once

#include "ShapeShader.h"

namespace cg {
class MeshShapeShader : public ShapeShader {
public:
    MeshData generateMesh(const Shape& shape) const override;
};
} // namespace cg
