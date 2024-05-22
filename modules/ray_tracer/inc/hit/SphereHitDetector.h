#pragma once

#include "hit/HitDetector.h"

#include "glm/mat3x3.hpp"

namespace cg {
class Sphere;

class SphereHitDetector : public HitDetector {
public:
    std::optional<HitDesc> hit(const Ray& ray, float rayMin, float rayMax) const override;
    void initForFrame() override;

private:
    HitDesc formHitDesc(const Ray& originalRay, const Ray& localizedRay, float rayHitVal, bool isOriginOutside) const;

    const Sphere* sphere_;
    glm::mat3 transposedLocalFrame_;
};
} // namespace cg
