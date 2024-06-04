#pragma once

#include "shader/ShapeShader.h"

namespace cg {
class SphereShapeShader : public ShapeShader {
public:
    SphereShapeShader(unsigned verticalSegCount, unsigned horizontalSegCount)
        : verticalSegCount_(verticalSegCount), horizontalSegCount_(horizontalSegCount) {}
    MeshData generateMesh(const Shape& shape) const override;

private:
    unsigned verticalSegCount_;
    unsigned horizontalSegCount_;
};
} // namespace cg
