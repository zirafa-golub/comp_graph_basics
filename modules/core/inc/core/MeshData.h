#pragma once

#include "core/BasicTypes.h"

#include <array>
#include <span>
#include <vector>

namespace cg {

class MeshData {
public:
    using Index = int;
    static constexpr Index invalidIndex = -1;

    struct VertexData {
        Index vertex;
        Index vertexNormal;

        bool operator==(const VertexData&) const = default;
        bool operator!=(const VertexData&) const = default;
    };

    using TriangleData = std::array<VertexData, 3>;

    static TriangleData createTriangle(Index vertex1, Index vertex2, Index vertex3) {
        return TriangleData({{vertex1, vertex1}, {vertex2, vertex2}, {vertex3, vertex3}});
    }
    static TriangleData createTriangle(Index vertex1, Index vertex2, Index vertex3, Index vertexNormal) {
        return TriangleData({{vertex1, vertexNormal}, {vertex2, vertexNormal}, {vertex3, vertexNormal}});
    }
    static TriangleData createTriangle(VertexData vertex1, VertexData vertex2, VertexData vertex3) {
        return TriangleData({vertex1, vertex2, vertex3});
    }

    MeshData(std::vector<Point> vertices, std::vector<glm::vec3> vertexNormals, std::vector<TriangleData> triangles)
        : vertices_(std::move(vertices)), vertexNormals_(std::move(vertexNormals)), triangles_(std::move(triangles)) {}

    std::span<const Point> vertices() const { return vertices_; }
    std::span<const glm::vec3> vertexNormals() const { return vertexNormals_; }
    std::span<const TriangleData> triangles() const { return triangles_; }

private:
    std::vector<Point> vertices_;
    std::vector<glm::vec3> vertexNormals_;
    std::vector<TriangleData> triangles_;
};

using VertexData = MeshData::VertexData;
using TriangleData = MeshData::TriangleData;
} // namespace cg
