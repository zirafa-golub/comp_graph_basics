#include "core/Mesh.h"
#include "shader/MeshShapeShader.h"

namespace cg {
MeshData MeshShapeShader::generateMesh(const Shape& shape) const {
    const Mesh& mesh = static_cast<const Mesh&>(shape);
    return mesh.meshData();
}
} // namespace cg
