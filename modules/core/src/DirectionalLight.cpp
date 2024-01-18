#include "core/DirectionalLight.h"

#include "glm/geometric.hpp"

namespace cg {
DirectionalLight::DirectionalLight(const glm::vec3& direction, const Color& intensity)
    : direction_(direction), intensity_(intensity) {}

const Color& DirectionalLight::intensity() const { return intensity_; }
void DirectionalLight::setIntensity(const Color& intensity) { intensity_ = intensity; }
const glm::vec3& DirectionalLight::direction() const { return direction_; }
void DirectionalLight::setDirection(const glm::vec3& direction) { direction_ = direction; }

Color DirectionalLight::illuminate(const HitDesc& pointDesc) const {
    float geometricFactor = std::max(0.0f, glm::dot(pointDesc.unitNormal, -direction_));
    Color irradiance = intensity_ * geometricFactor;
    Color reflectedLightFactor =
        pointDesc.hitShape->material().reflect(pointDesc.unitNormal, -pointDesc.ray.direction(), -direction_);

    return irradiance * reflectedLightFactor;
}
} // namespace cg
