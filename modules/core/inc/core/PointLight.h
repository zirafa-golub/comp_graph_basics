#pragma once

#include "core/Color.h"
#include "core/Light.h"

namespace cg {
class PointLight : public Light, public Movable {
public:
    PointLight(const Color& intensity = Color(1, 1, 1));

    const Color& intensity();
    void setIntensity(const Color& intensity);

    Color illuminate(const HitDesc& pointDesc) const override;

private:
    Color intensity_;
};
} // namespace cg