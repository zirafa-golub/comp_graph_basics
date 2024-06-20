#include "rasterizer/RasterizerRendererParallel.h"

#include "core/Camera.h"

namespace cg {
void RasterizerRendererParallel::prepareBuffers(int targetWidth, int targetHeight) {
    if (colorBuffers_.empty() || colorBuffers_[0].width() != targetWidth || colorBuffers_[0].height() != targetHeight) {
        colorBuffers_.clear();
        depthBuffers_.clear();
        for (unsigned i = 0; i < threadPool_.threadCount(); ++i) {
            depthBuffers_.emplace_back(targetWidth, targetHeight);
        }
        // Create one less color buffer since thread 0 writes to target directly
        for (unsigned i = 0; i < threadPool_.threadCount() - 1; ++i) {
            colorBuffers_.emplace_back(targetWidth, targetHeight);
        }
    }
}

void RasterizerRendererParallel::meshToGlobalSpace(MeshData& mesh, const Shape& shape) {
    const auto& toGlobalMatrix = shape.toGlobalFrameMatrix();
    for (Point& vertex : mesh.vertices()) {
        vertex = toGlobalMatrix * glm::vec4(vertex, 1.0f);
    }

    glm::mat4 toGlobalNormalMatrix = glm::transpose(shape.toLocalFrameMatrix());
    for (glm::vec3& normal : mesh.vertexNormals()) {
        normal = glm::normalize(toGlobalNormalMatrix * glm::vec4(normal, 0.0f));
    }
}

MeshData RasterizerRendererParallel::cullBackFaceTriangles(MeshData& mesh, const BackFaceCuller& culler) {
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

std::vector<float> RasterizerRendererParallel::verticesToScreenSpace(const glm::mat4& toScreenMatrix, MeshData& mesh) {
    std::vector<float> invW;
    invW.reserve(mesh.vertices().size());

    for (Point& point : mesh.vertices()) {
        glm::vec4 transformedVertex = toScreenMatrix * glm::vec4(point, 1.0f);
        invW.push_back(1 / transformedVertex.w);
        point = transformedVertex;
        point /= transformedVertex.w;
    }
    return invW;
}
} // namespace cg
