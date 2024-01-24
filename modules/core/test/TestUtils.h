#pragma once

#include "core/Material.h"
#include "core/Shape.h"

#include <expected>

namespace cg {
class Color;
}

void assertColorsFloatEqual(const cg::Color& left, const cg::Color& right);

class UnhittableShape : public cg::Shape {
public:
    std::expected<cg::HitDesc, cg::Error> hit(const cg::Ray& ray, float tMin, float tMax) const override;
};

class PerfectMirrorMaterial : public cg::Material {
public:
    cg::Color reflect(const glm::vec3& normal, const glm::vec3& viewerDirection,
                      const glm::vec3& lightDirection) const override;
};
