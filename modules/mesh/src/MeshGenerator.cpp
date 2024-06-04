#include "core/Angle.h"
#include "mesh/MeshGenerator.h"

#include "glm/geometric.hpp"

#include <cassert>
#include <cmath>
#include <memory>

using namespace cg::angle_literals;

namespace cg {
MeshData MeshGenerator::generateSphere(float radius, unsigned verticalSegCount, unsigned horizontalSegCount) {
    assert(verticalSegCount >= 2);
    assert(horizontalSegCount >= 3);

    Angle verticalStep = 180_deg / verticalSegCount;
    Angle horizontalStep = 360_deg / horizontalSegCount;

    std::vector<Point> vertices;
    std::vector<glm::vec3> vertexNormals;
    std::vector<TriangleData> triangles;

    // Generate "upper cap"
    vertices.push_back(generateSpherePoint(radius, 0_deg, 0_deg));
    vertexNormals.push_back(glm::normalize(vertices.back()));
    for (unsigned hor = 0; hor < horizontalSegCount; ++hor) {
        vertices.push_back(generateSpherePoint(radius, verticalStep, hor * horizontalStep));
        vertexNormals.push_back(glm::normalize(vertices.back()));
    }
    for (unsigned hor = 0; hor < horizontalSegCount; ++hor) {
        triangles.push_back(MeshData::createTriangle(hor, (hor + 1) % horizontalSegCount, 0));
    }

    // Generate mid part
    for (unsigned ver = 1; ver < verticalSegCount - 1; ++ver) {
        Angle verticalAngle = (ver + 1) * verticalStep;
        unsigned segStart = static_cast<unsigned>(vertices.size());
        for (unsigned hor = 0; hor < horizontalSegCount; ++hor) {
            vertices.push_back(generateSpherePoint(radius, verticalAngle, hor * horizontalStep));
            vertexNormals.push_back(glm::normalize(vertices.back()));
        }
        for (unsigned hor = 0; hor < horizontalSegCount; ++hor) {
            unsigned currVer = segStart + hor;
            unsigned nextVer = segStart + ((hor + 1) % horizontalSegCount);
            triangles.push_back(MeshData::createTriangle(currVer, nextVer, currVer - horizontalSegCount));
            triangles.push_back(
                MeshData::createTriangle(nextVer, nextVer - horizontalSegCount, currVer - horizontalSegCount));
        }
    }

    // Generate "lower cap"
    unsigned segStart = static_cast<unsigned>(vertices.size() - horizontalSegCount);
    vertices.push_back(generateSpherePoint(radius, 180_deg, 0_deg));
    vertexNormals.push_back(glm::normalize(vertices.back()));
    unsigned lowerPolarVer = static_cast<unsigned>(vertices.size() - 1);
    for (unsigned hor = 0; hor < horizontalSegCount; ++hor) {
        triangles.push_back(
            MeshData::createTriangle(segStart + hor, lowerPolarVer, segStart + ((hor + 1) % horizontalSegCount)));
    }

    return MeshData(std::move(vertices), std::move(vertexNormals), std::move(triangles));
}

Point MeshGenerator::generateSpherePoint(float radius, Angle verticalAngle, Angle horizontalAngle) {
    float vRads = verticalAngle.asRad();
    float hRads = horizontalAngle.asRad();
    return Point(radius * std::cos(hRads) * std::sin(vRads), radius * std::cos(vRads),
                 -radius * std::sin(hRads) * std::sin(vRads));
}

MeshData MeshGenerator::generateRectangle(Size2d size, unsigned horSegCount, unsigned verSegCount) {
    assert(horSegCount > 0 && verSegCount > 0);
    assert(size.width > 0 && size.height > 0);

    Point lowerLeftCorner = Point(-size.width / 2, -size.height / 2, 0);
    glm::vec3 rightIncrement = {size.width / horSegCount, 0, 0};
    glm::vec3 upIncrement = {0, size.height / verSegCount, 0};
    glm::vec3 normal = {0, 0, 1};
    const unsigned verticesPerRow = horSegCount + 1;
    std::vector<Point> vertices;
    std::vector<TriangleData> triangles;

    // generate lower edge
    for (unsigned i = 0; i < verticesPerRow; ++i) {
        vertices.push_back(lowerLeftCorner + static_cast<float>(i) * rightIncrement);
    }

    // generate the rest, row by row
    for (unsigned i = 0; i < verSegCount; ++i) {
        Point rowStart = lowerLeftCorner + static_cast<float>(i + 1) * upIncrement;
        // generate edge
        for (unsigned j = 0; j < verticesPerRow; ++j) {
            vertices.push_back(rowStart + static_cast<float>(j) * rightIncrement);
        }

        // generate triangles
        unsigned currEdgeStart = static_cast<unsigned>(vertices.size()) - verticesPerRow;
        unsigned previousEdgeStart = currEdgeStart - verticesPerRow;
        for (unsigned j = 0; j < horSegCount; ++j) {
            unsigned lowerLeftVertex = previousEdgeStart + j;
            unsigned lowerRightVertex = previousEdgeStart + j + 1;
            unsigned upperLeftVertex = currEdgeStart + j;
            unsigned upperRightVertex = currEdgeStart + j + 1;
            triangles.push_back(MeshData::createTriangle(lowerLeftVertex, lowerRightVertex, upperLeftVertex, 0));
            triangles.push_back(MeshData::createTriangle(lowerRightVertex, upperRightVertex, upperLeftVertex, 0));
        }
    }

    return MeshData(std::move(vertices), std::vector<glm::vec3>({normal}), std::move(triangles));
}
} // namespace cg
