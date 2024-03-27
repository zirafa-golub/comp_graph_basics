#pragma once

#include "core/BasicTypes.h"

#include <array>
#include <vector>

namespace cg {
class MeshData {
public:
    using TriangleIndices = std::array<unsigned, 3>;

    MeshData(std::vector<Point> vertices, std::vector<TriangleIndices> triangles)
        : vertices_(std::move(vertices)), triangles_(std::move(triangles)) {}

    const std::vector<Point>& vertices() const { return vertices_; }
    const std::vector<TriangleIndices>& triangles() const { return triangles_; }

private:
    std::vector<Point> vertices_;
    std::vector<TriangleIndices> triangles_;
};
} // namespace cg
