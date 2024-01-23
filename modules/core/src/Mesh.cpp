#include "core/Mesh.h"

#include "common/Math.h"

#include "glm/geometric.hpp"
#include "glm/gtx/component_wise.hpp"

namespace cg {
Mesh::Mesh(std::vector<Point> vertices, std::vector<TriangleIndices> triangles)
    : vertices_(std::move(vertices)), triangles_(std::move(triangles)) {}

const std::vector<Point>& Mesh::vertices() const { return vertices_; }
const std::vector<Mesh::TriangleIndices>& Mesh::triangles() const { return triangles_; }

std::expected<HitDesc, Error> Mesh::hit(const Ray& ray, float tMin, float tMax) const {
    float closestHit = std::numeric_limits<float>::max();
    glm::vec3 unitNormal;

    for (const auto& triangle : triangles_) {
        const auto& vertexA = vertices_[triangle[0]] + position();
        const auto& vertexB = vertices_[triangle[1]] + position();
        const auto& vertexC = vertices_[triangle[2]] + position();
        auto triHitResult = hitTriangle(ray, tMin, tMax, vertexA, vertexB, vertexC);

        if (triHitResult.has_value()) {
            closestHit = triHitResult.value().tHit;
            unitNormal = glm::normalize(glm::cross(vertexB - vertexA, vertexC - vertexA));
            tMax = closestHit;
        }
    }

    if (closestHit == std::numeric_limits<float>::max()) {
        return std::unexpected(ErrorCode::NotFound);
    }

    return HitDesc(this, ray, closestHit, unitNormal);
}

std::expected<Mesh::TriangleHit, ErrorCode> Mesh::hitTriangle(const Ray& ray, float tMin, float tMax,
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
        return std::unexpected(ErrorCode::NotFound);
    }

    float beta = glm::compAdd(jkl * glm::vec3(aPrim, bPrim, cPrim)) / M;
    if (!isInRangeIncl(beta, 0.0f, 1.0f)) {
        return std::unexpected(ErrorCode::NotFound);
    }

    float dPrim = abc[0] * jkl[1] - jkl[0] * abc[1];
    float ePrim = jkl[0] * abc[2] - abc[0] * jkl[2];
    float fPrim = abc[1] * jkl[2] - jkl[1] * abc[2];

    float gamma = glm::compAdd(ghi * glm::vec3(fPrim, ePrim, dPrim)) / M;
    if (!isInRangeIncl(gamma, 0.0f, 1 - beta)) {
        return std::unexpected(ErrorCode::NotFound);
    }

    float tHit = -glm::compAdd(def * glm::vec3(fPrim, ePrim, dPrim)) / M;

    if (!isInRangeIncl(tHit, tMin, tMax)) {
        return std::unexpected(ErrorCode::NotFound);
    }

    return TriangleHit{tHit, beta, gamma};
}
} // namespace cg