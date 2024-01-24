#pragma once

#include "core/Light.h"

namespace cg {
class AmbientLight : public Light {
public:
    AmbientLight(const Color& intensity) : intensity_(intensity) {}

    virtual Color illuminate(const HitDesc& pointDesc) const override {
        return intensity_ * pointDesc.hitShape->ambientReflectance();
    }
    DistanceDesc distanceFrom(const Point& point) const override { return DistanceDesc(0.0f, glm::vec3(0, 0, 0)); }

    const Color& intensity() const { return intensity_; }
    void setIntensity(const Color& newIntensity) { intensity_ = newIntensity; }

private:
    Color intensity_;
};
} // namespace cg
