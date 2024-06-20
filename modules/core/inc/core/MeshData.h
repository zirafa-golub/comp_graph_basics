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

    struct Data {
        std::vector<Point> vertices;
        std::vector<glm::vec3> vertexNormals;
        std::vector<TriangleData> triangles;

        Data() = default;
        Data(std::vector<Point> vertices_, std::vector<glm::vec3> vertexNormals_, std::vector<TriangleData> triangles_)
            : vertices(std::move(vertices_)), vertexNormals(std::move(vertexNormals_)),
              triangles(std::move(triangles_)) {}
    };

    static TriangleData createTriangle(Index vertex1, Index vertex2, Index vertex3) {
        return TriangleData({{vertex1, vertex1}, {vertex2, vertex2}, {vertex3, vertex3}});
    }
    static TriangleData createTriangle(Index vertex1, Index vertex2, Index vertex3, Index vertexNormal) {
        return TriangleData({{vertex1, vertexNormal}, {vertex2, vertexNormal}, {vertex3, vertexNormal}});
    }
    static TriangleData createTriangle(VertexData vertex1, VertexData vertex2, VertexData vertex3) {
        return TriangleData({vertex1, vertex2, vertex3});
    }

    MeshData() = default;

    MeshData(std::vector<Point> vertices, std::vector<glm::vec3> vertexNormals, std::vector<TriangleData> triangles)
        : data_(std::move(vertices), std::move(vertexNormals), std::move(triangles)) {}
    MeshData(Data data) : data_(std::move(data)) {}

    std::span<const Point> vertices() const { return data_.vertices; }
    std::span<Point> vertices() { return data_.vertices; }
    std::span<const glm::vec3> vertexNormals() const { return data_.vertexNormals; }
    std::span<glm::vec3> vertexNormals() { return data_.vertexNormals; }
    std::span<const TriangleData> triangles() const { return data_.triangles; }
    std::span<TriangleData> triangles() { return data_.triangles; }
    Data claimData() { return std::move(data_); }

private:
    Data data_;
};

using VertexData = MeshData::VertexData;
using TriangleData = MeshData::TriangleData;
} // namespace cg
