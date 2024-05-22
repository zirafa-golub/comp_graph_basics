#pragma once

#include "core/Color.h"
#include "core/Light.h"

namespace cg {
class DirectionalLight : public Light {
public:
    DirectionalLight(const glm::vec3& direction = {0, -1, 0}, const Color& intensity = Color(1, 1, 1));

    const Color& intensity() const;
    void setIntensity(const Color& intensity);
    const glm::vec3& direction() const;
    void setDirection(const glm::vec3& direction);

    Color illuminate(const Point& illuminatedPoint, const glm::vec3& unitNormal) const override;
    DistanceDesc distanceFrom(const Point& point) const override;

private:
    glm::vec3 direction_;
    Color intensity_;
};
} // namespace cg
