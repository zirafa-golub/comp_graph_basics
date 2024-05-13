#include "core/Angle.h"
#include "core/Mesh.h"
#include "core/Sphere.h"

#include "common/Math.h"

#include "glm/geometric.hpp"

using namespace cg::angle_literals;

namespace cg {
Sphere::Sphere(float radius) : radius_(radius), transposedLocalFrame_(glm::transpose(toLocalFrameMatrix())) {}

const Point& Sphere::center() const { return position(); }

float Sphere::radius() const { return radius_; }

std::expected<HitDesc, Error> Sphere::hit(const Ray& ray, float tMin, float tMax) const {
    const auto& toLocalFrame = toLocalFrameMatrix();
    glm::vec3 localizedOrigin = toLocalFrame * glm::vec4(ray.origin(), 1);
    glm::vec3 localizedDirection = glm::mat3(toLocalFrame) * ray.direction();
    Ray localizedRay(localizedOrigin, localizedDirection);

    // since we're doing hit detection in sphere's local frame, we don't need to figure in the sphere center because
    // it's alsways in frame origin (it's [0, 0, 0])
    const glm::vec3& centerToOrigin = localizedRay.origin();
    float radiusSquared = radius_ * radius_;
    float centerToOriginSquared = glm::dot(centerToOrigin, centerToOrigin);

    // calulate quadratic equation parameters
    float a = glm::dot(localizedRay.direction(), localizedRay.direction());
    float b = 2 * glm::dot(localizedRay.direction(), centerToOrigin);
    float c = centerToOriginSquared - radiusSquared;

    QuadSolve quadSolve = solveQuadEquation(a, b, c);

    if (quadSolve.count > 0) {
        bool isOriginOutside = radiusSquared < centerToOriginSquared;
        if (isInRangeIncl(quadSolve.solutions[0], tMin, tMax)) {
            return formHitDesc(ray, localizedRay, quadSolve.solutions[0], isOriginOutside);
        } else if (quadSolve.count == 2 && isInRangeIncl(quadSolve.solutions[1], tMin, tMax)) {
            return formHitDesc(ray, localizedRay, quadSolve.solutions[1], isOriginOutside);
        }
    }

    return std::unexpected(ErrorCode::NotFound);
};

void Sphere::transformUpdated() { transposedLocalFrame_ = glm::transpose(toLocalFrameMatrix()); }

HitDesc Sphere::formHitDesc(const Ray& originalRay, const Ray& localizedRay, float tHit, bool isOriginOutside) const {
    Point hitPoint = localizedRay.evaluate(tHit);

    glm::vec3 localizedNormal = isOriginOutside ? hitPoint : -hitPoint;
    // To transform the normal vector to global frame, we need the transposed inverse of the to-global-frame transform
    // which is transposed to-local-frame transform
    glm::vec3 unitNormal = glm::normalize(transposedLocalFrame_ * localizedNormal);

    return HitDesc{this, originalRay, tHit, unitNormal};
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
