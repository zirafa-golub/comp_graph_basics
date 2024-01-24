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

    Color illuminate(const HitDesc& pointDesc) const override;

private:
    glm::vec3 direction_;
    Color intensity_;
};
} // namespace cg
