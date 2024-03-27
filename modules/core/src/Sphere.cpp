#include "core/Angle.h"
#include "core/Mesh.h"
#include "core/Sphere.h"

#include "common/Math.h"

#include "glm/geometric.hpp"

using namespace cg::angle_literals;

namespace cg {
const Point& Sphere::center() const { return position(); }

float Sphere::radius() const { return radius_; }

std::expected<HitDesc, Error> Sphere::hit(const Ray& ray, float tMin, float tMax) const {
    glm::vec3 originMinusCenter = ray.origin() - center();
    float radiusSquared = radius_ * radius_;
    float centerToOriginSquared = glm::dot(originMinusCenter, originMinusCenter);

    // calulate quadratic equation parameters
    float a = glm::dot(ray.direction(), ray.direction());
    float b = 2 * glm::dot(ray.direction(), originMinusCenter);
    float c = centerToOriginSquared - radiusSquared;

    QuadSolve quadSolve = solveQuadEquation(a, b, c);

    if (quadSolve.count > 0) {
        bool isOriginOutside = radiusSquared < centerToOriginSquared;
        if (isInRangeIncl(quadSolve.solutions[0], tMin, tMax)) {
            return formHitDesc(ray, quadSolve.solutions[0], isOriginOutside);
        } else if (quadSolve.count == 2 && isInRangeIncl(quadSolve.solutions[1], tMin, tMax)) {
            return formHitDesc(ray, quadSolve.solutions[1], isOriginOutside);
        }
    }

    return std::unexpected(ErrorCode::NotFound);
};

HitDesc Sphere::formHitDesc(const Ray& ray, float tHit, bool isOriginOutside) const {
    Point hitPoint = ray.evaluate(tHit);
    glm::vec3 unitNormal = isOriginOutside ? (hitPoint - center()) / radius_ : (center() - hitPoint) / radius_;

    return HitDesc{this, ray, tHit, unitNormal};
}

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
    std::vector<Mesh::TriangleIndices> triangles;

    // Generate "upper cap"
    vertices.push_back(generatePoint(0_deg, 0_deg));
    for (unsigned hor = 0; hor < horizontalSegCount; ++hor) {
        vertices.push_back(generatePoint(verticalStep, hor * horizontalStep));
    }
    for (unsigned hor = 0; hor < horizontalSegCount; ++hor) {
        triangles.push_back(Mesh::TriangleIndices({hor, (hor + 1) % horizontalSegCount, 0}));
    }

    // Generate mid part
    for (unsigned ver = 1; ver < verticalSegCount - 1; ++ver) {
        Angle verticalAngle = (ver + 1) * verticalStep;
        unsigned segStart = static_cast<unsigned>(vertices.size());
        for (unsigned hor = 0; hor < horizontalSegCount; ++hor) {
            vertices.push_back(generatePoint(verticalAngle, hor * horizontalStep));
        }
        for (unsigned hor = 0; hor < horizontalSegCount; ++hor) {
            unsigned currVer = segStart + hor;
            unsigned nextVer = segStart + ((hor + 1) % horizontalSegCount);
            triangles.push_back(Mesh::TriangleIndices({currVer, nextVer, currVer - horizontalSegCount}));
            triangles.push_back(
                Mesh::TriangleIndices({nextVer, nextVer - horizontalSegCount, currVer - horizontalSegCount}));
        }
    }

    // Generate "lower cap"
    unsigned segStart = static_cast<unsigned>(vertices.size() - horizontalSegCount);
    vertices.push_back(generatePoint(180_deg, 0_deg));
    unsigned lowerPolarVer = static_cast<unsigned>(vertices.size() - 1);
    for (unsigned hor = 0; hor < horizontalSegCount; ++hor) {
        triangles.push_back(
            Mesh::TriangleIndices({segStart + hor, lowerPolarVer, segStart + ((hor + 1) % horizontalSegCount)}));
    }

    meshData_ = std::make_unique<MeshData>(std::move(vertices), std::move(triangles));
}

Point Sphere::generatePoint(Angle verticalAngle, Angle horizontalAngle) const {
    float vRads = verticalAngle.asRad();
    float hRads = horizontalAngle.asRad();
    return Point(radius_ * std::cos(hRads) * std::sin(vRads), radius_ * std::cos(vRads),
                 -radius_ * std::sin(hRads) * std::sin(vRads));
}
} // namespace cg
