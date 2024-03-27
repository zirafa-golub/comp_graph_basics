#pragma once

#include "core/Material.h"
#include "core/Shape.h"

#include "glm/vec3.hpp"

#include <expected>

namespace cg {
class Color;
}

void assertColorsFloatEqual(const cg::Color& left, const cg::Color& right);
void assertVec3FloatEqual(const glm::vec3& left, const glm::vec3& right);
void areVectorsParallel(const glm::vec3& left, const glm::vec3& right);

class UnhittableShape : public cg::Shape {
public:
    std::expected<cg::HitDesc, cg::Error> hit(const cg::Ray& ray, float tMin, float tMax) const override;
    const cg::MeshData& meshData() const override;

private:
    cg::MeshData meshData_ = cg::MeshData({}, {});
};

class PerfectMirrorMaterial : public cg::Material {
public:
    cg::Color reflect(const glm::vec3& normal, const glm::vec3& viewerDirection,
                      const glm::vec3& lightDirection) const override;
    cg::Color surfaceReflectance() const override;
};
