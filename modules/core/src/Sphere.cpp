#include "core/Angle.h"
#include "core/Mesh.h"
#include "core/Sphere.h"

#include "common/Math.h"

#include "glm/geometric.hpp"

using namespace cg::angle_literals;

namespace cg {
Sphere::Sphere(float radius) : radius_(radius) {}

const Point& Sphere::center() const { return position(); }

float Sphere::radius() const { return radius_; }

const MeshData& Sphere::meshData() const {
    assert(meshData_ != nullptr && "Call generateMesh first!");
    return *meshData_;
}

void Sphere::generateMesh(unsigned verticalSegCount, unsigned horizontalSegCount) {
    assert(verticalSegCount >= 2);
    assert(horizontalSegCount >= 3);

    Angle verticalStep = 180_deg / verticalSegCount;
    Angle horizontalStep = 360_deg / horizontalSegCount;

    std::vector<Point> vertices;
    std::vector<glm::vec3> vertexNormals;
    std::vector<TriangleData> triangles;

    // Generate "upper cap"
    vertices.push_back(generatePoint(0_deg, 0_deg));
    vertexNormals.push_back(glm::normalize(vertices.back()));
    for (unsigned hor = 0; hor < horizontalSegCount; ++hor) {
        vertices.push_back(generatePoint(verticalStep, hor * horizontalStep));
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
            vertices.push_back(generatePoint(verticalAngle, hor * horizontalStep));
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
    vertices.push_back(generatePoint(180_deg, 0_deg));
    vertexNormals.push_back(glm::normalize(vertices.back()));
    unsigned lowerPolarVer = static_cast<unsigned>(vertices.size() - 1);
    for (unsigned hor = 0; hor < horizontalSegCount; ++hor) {
        triangles.push_back(
            MeshData::createTriangle(segStart + hor, lowerPolarVer, segStart + ((hor + 1) % horizontalSegCount)));
    }

    meshData_ = std::make_unique<MeshData>(std::move(vertices), std::move(vertexNormals), std::move(triangles));
}

Point Sphere::generatePoint(Angle verticalAngle, Angle horizontalAngle) const {
    float vRads = verticalAngle.asRad();
    float hRads = horizontalAngle.asRad();
    return Point(radius_ * std::cos(hRads) * std::sin(vRads), radius_ * std::cos(vRads),
                 -radius_ * std::sin(hRads) * std::sin(vRads));
}
} // namespace cg
