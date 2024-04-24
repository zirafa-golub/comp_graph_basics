#pragma once

#include "core/BasicTypes.h"

#include <array>
#include <span>
#include <vector>

namespace cg {
class MeshData {
public:
    MeshData(std::vector<Point> vertices, std::vector<TriangleIndices> triangles)
        : vertices_(std::move(vertices)), triangles_(std::move(triangles)) {}

    std::span<const Point> vertices() const { return vertices_; }
    std::span<const TriangleIndices> triangles() const { return triangles_; }

private:
    std::vector<Point> vertices_;
    std::vector<TriangleIndices> triangles_;
};
} // namespace cg
