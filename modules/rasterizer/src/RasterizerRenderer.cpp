#include "rasterizer/RasterizerRenderer.h"

#include "core/Camera.h"

namespace cg {

void RasterizerRenderer::meshToGlobalSpace(MeshData& mesh, const Shape& shape) {
    const auto& toGlobalMatrix = shape.toGlobalFrameMatrix();
    for (Point& vertex : mesh.vertices()) {
        vertex = toGlobalMatrix * glm::vec4(vertex, 1.0f);
    }

    glm::mat4 toGlobalNormalMatrix = glm::transpose(shape.toLocalFrameMatrix());
    for (glm::vec3& normal : mesh.vertexNormals()) {
        normal = glm::normalize(toGlobalNormalMatrix * glm::vec4(normal, 0.0f));
    }
}

MeshData RasterizerRenderer::cullBackFaceTriangles(MeshData& mesh, const BackFaceCuller& culler) {
    MeshData::Data meshData = mesh.claimData();
    const auto& vertices = meshData.vertices;
    std::vector<TriangleData> nonCulledTriangles;
    nonCulledTriangles.reserve(meshData.triangles.size());
    for (const auto& triangle : meshData.triangles) {
        if (!culler.shouldCull(vertices[triangle[0].vertex], vertices[triangle[1].vertex],
                               vertices[triangle[2].vertex])) {
            nonCulledTriangles.push_back(triangle);
        }
    }
    return MeshData(std::move(meshData.vertices), std::move(meshData.vertexNormals), std::move(nonCulledTriangles));
}

void RasterizerRenderer::verticesToScreenSpace(const glm::mat4& toScreenMatrix, MeshData& mesh) {
    std::vector<glm::vec4> screenSpaceVertices;

    for (Point& point : mesh.vertices()) {
        glm::vec4 transformedVertex = toScreenMatrix * glm::vec4(point, 1.0f);
        point = transformedVertex / transformedVertex.w;
    }
}
} // namespace cg
