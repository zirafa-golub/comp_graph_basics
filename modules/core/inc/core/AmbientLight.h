#pragma once

#include "core/Light.h"

namespace cg {
class AmbientLight : public Light {
public:
    AmbientLight(const Color& intensity) : intensity_(intensity) {}

    virtual Color illuminate(const HitDesc& pointDesc) const override {
        return intensity_ * pointDesc.hitShape->ambientReflectance();
    }

    const Color& intensity() const { return intensity_; }
    void setIntensity(const Color& newIntensity) { intensity_ = newIntensity; }

private:
    Color intensity_;
};
} // namespace cg
