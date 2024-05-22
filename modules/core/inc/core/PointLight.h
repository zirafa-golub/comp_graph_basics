#pragma once

#include "core/Color.h"
#include "core/Light.h"
#include "core/Movable.h"

namespace cg {
class PointLight : public Light, public Movable {
public:
    PointLight(const Color& intensity = Color(1, 1, 1));

    const Color& intensity();
    void setIntensity(const Color& intensity);

    Color illuminate(const Point& illuminatedPoint, const glm::vec3& unitNormal) const override;
    DistanceDesc distanceFrom(const Point& point) const override;

private:
    Color intensity_;
};
} // namespace cg
