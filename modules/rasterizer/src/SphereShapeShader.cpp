#include "shader/SphereShapeShader.h"

#include "core/Sphere.h"
#include "mesh/MeshGenerator.h"

namespace cg {
MeshData SphereShapeShader::generateMesh(const Shape& shape) const {
    const Sphere& sphere = static_cast<const Sphere&>(shape);
    return MeshGenerator::generateSphere(sphere.radius(), verticalSegCount_, horizontalSegCount_);
}
} // namespace cg
