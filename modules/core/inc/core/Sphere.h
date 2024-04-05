#pragma once

#include "BasicTypes.h"
#include "Shape.h"

#include "glm/mat4x4.hpp"

#include <array>
#include <cmath>
#include <expected>

namespace cg {
class Angle;
class Sphere : public Shape {
public:
    Sphere(float radius) : radius_(radius) {}

    const Point& center() const;

    float radius() const;

    std::expected<HitDesc, Error> hit(const Ray& ray, float tMin, float tMax) const override;
    const MeshData& meshData() const override;
    void generateMesh(unsigned verticalSegments, unsigned horizontalSegments);

protected:
    void transformUpdated() override;

private:
    HitDesc formHitDesc(const Ray& originalRay, const Ray& localizedRay, float tHit, bool isOriginOutside) const;
    Point generatePoint(Angle verticalAngle, Angle horizontalAngle) const;

    float radius_;
    glm::mat3 transposedLocalFrame_;
    std::unique_ptr<MeshData> meshData_;
};
} // namespace cg
