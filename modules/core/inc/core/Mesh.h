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

    Mesh(std::vector<Point> vertices, std::vector<TriangleIndices> triangles);

    const std::vector<Point>& vertices() const;
    const std::vector<TriangleIndices>& triangles() const;

    std::expected<HitDesc, Error> hit(const Ray& ray, float tMin, float tMax) const override;
    static std::expected<TriangleHit, ErrorCode> hitTriangle(const Ray& ray, float tMin, float tMax,
                                                             const Point& vertexA, const Point& vertexB,
                                                             const Point& vertexC);

private:
    std::vector<Point> vertices_;
    std::vector<TriangleIndices> triangles_;
};
} // namespace cg