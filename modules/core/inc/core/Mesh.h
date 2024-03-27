#pragma once

#include "core/Shape.h"

#include <array>

namespace cg {

class Mesh : public Shape {
public:
    struct TriangleHit {
        float tHit;
        float beta;
        float gamma;
    };

    using TriangleIndices = std::array<unsigned, 3>;

    Mesh(MeshData meshData);

    std::expected<HitDesc, Error> hit(const Ray& ray, float tMin, float tMax) const override;
    static std::expected<TriangleHit, ErrorCode> hitTriangle(const Ray& ray, float tMin, float tMax,
                                                             const Point& vertexA, const Point& vertexB,
                                                             const Point& vertexC);
    const MeshData& meshData() const override;

private:
    MeshData meshData_;
};
} // namespace cg
