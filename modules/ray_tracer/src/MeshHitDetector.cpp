#include "common/Math.h"
#include "core/Mesh.h"
#include "core/MeshData.h"
#include "hit/MeshHitDetector.h"

#include "glm/geometric.hpp"
#include "glm/gtx/component_wise.hpp"
#include "glm/vec4.hpp"

namespace cg {
void MeshHitDetector::initForFrame() {
    mesh_ = static_cast<const Mesh*>(&shaderGroup().shape());

    const MeshData& meshData = mesh_->meshData();

    const auto& vertices = meshData.vertices();
    transformedVertices_.resize(vertices.size());
    const auto& toGlobal = mesh_->toGlobalFrameMatrix();
    for (int i = 0; i < vertices.size(); ++i) {
        transformedVertices_[i] = toGlobal * glm::vec4(vertices[i], 1.0f);
    }

    glm::mat3 transposedLocalFrame = glm::transpose(mesh_->toLocalFrameMatrix());
    const auto& normals = meshData.vertexNormals();
    transformedNormals_.resize(normals.size());
    for (int i = 0; i < normals.size(); ++i) {
        transformedNormals_[i] = transposedLocalFrame * normals[i];
    }
}

std::optional<HitDesc> MeshHitDetector::hit(const Ray& ray, float rayMin, float rayMax) const {
    float closestHit = std::numeric_limits<float>::max();
    glm::vec3 unitNormal;

    for (const auto& triangle : mesh_->meshData().triangles()) {
        auto triHitResult =
            hitTriangle(ray, rayMin, rayMax, transformedVertices_[triangle[0].vertex],
                        transformedVertices_[triangle[1].vertex], transformedVertices_[triangle[2].vertex]);

        if (triHitResult.has_value()) {
            const TriangleHit& hit = triHitResult.value();
            closestHit = hit.rayHitVal;
            float alpha = 1 - hit.beta - hit.gamma;
            // To transform the normal vector to global frame, we need the transposed inverse of the to-global-frame
            // transform which is transposed to-local-frame transform
            unitNormal = glm::normalize(alpha * glm::normalize(transformedNormals_[triangle[0].vertexNormal]) +
                                        hit.beta * glm::normalize(transformedNormals_[triangle[1].vertexNormal]) +
                                        hit.gamma * glm::normalize(transformedNormals_[triangle[2].vertexNormal]));
            rayMax = closestHit;
        }
    }

    if (closestHit == std::numeric_limits<float>::max()) {
        return std::nullopt;
    }

    return HitDesc(mesh_, ray, closestHit, unitNormal);
}

std::optional<MeshHitDetector::TriangleHit> MeshHitDetector::hitTriangle(const Ray& ray, float rayMin, float rayMax,
                                                                         const Point& vertexA, const Point& vertexB,
                                                                         const Point& vertexC) {
    /*  a = X_a - X_b   d = X_a - X_c   g = X_d     j = X_a - X_e
     *  b = Y_a - Y_b   e = Y_a - Y_c   h = Y_d     k = Y_a - Y_e
     *  c = Z_a - Z_b   f = Z_a - Z_c   i = Z_d     l = Z_a - Z_e
     *
     *  a' = e*i - h*f  d' = a*k - j*b
     *  b' = g*f - d*i  e' = j*c - a*l
     *  c' = d*h - e*g  f' = b*l - k*c
     *
     *  M = a*a' + b*b' + c*c'
     *
     *  t = (j*a' + k*b' + l*c')/M
     *  beta = (i*d' + h*e' + g*f')/M
     *  gamma = (f*d' + e*e' + d*f')/M
     */

    auto abc = vertexA - vertexB;
    auto def = vertexA - vertexC;
    auto ghi = ray.direction();
    auto jkl = vertexA - ray.origin();

    float aPrim = def[1] * ghi[2] - ghi[1] * def[2];
    float bPrim = ghi[0] * def[2] - def[0] * ghi[2];
    float cPrim = def[0] * ghi[1] - def[1] * ghi[0];

    float M = glm::compAdd(abc * glm::vec3(aPrim, bPrim, cPrim));
    if (M == 0) {
        return std::nullopt;
    }

    float beta = glm::compAdd(jkl * glm::vec3(aPrim, bPrim, cPrim)) / M;
    if (!isInRangeIncl(beta, 0.0f, 1.0f)) {
        return std::nullopt;
    }

    float dPrim = abc[0] * jkl[1] - jkl[0] * abc[1];
    float ePrim = jkl[0] * abc[2] - abc[0] * jkl[2];
    float fPrim = abc[1] * jkl[2] - jkl[1] * abc[2];

    float gamma = glm::compAdd(ghi * glm::vec3(fPrim, ePrim, dPrim)) / M;
    if (!isInRangeIncl(gamma, 0.0f, 1 - beta)) {
        return std::nullopt;
    }

    float rayHitVal = -glm::compAdd(def * glm::vec3(fPrim, ePrim, dPrim)) / M;

    if (!isInRangeIncl(rayHitVal, rayMin, rayMax)) {
        return std::nullopt;
    }

    return TriangleHit{rayHitVal, beta, gamma};
}
} // namespace cg
