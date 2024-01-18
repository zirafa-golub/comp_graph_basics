#include "core/PointLight.h"

#include "glm/geometric.hpp"

#include <numbers>

namespace cg {
PointLight::PointLight(const Color& intensity) : intensity_(intensity) {}

const Color& PointLight::intensity() { return intensity_; }
void PointLight::setIntensity(const Color& intensity) { intensity_ = intensity; }

Color PointLight::illuminate(const HitDesc& pointDesc) const {
    Point point = pointDesc.ray.evaluate(pointDesc.tHit);

    glm::vec3 pointToLight = position() - point;
    float lightDistance = glm::length(pointToLight);
    glm::vec3 lightDirection = pointToLight / lightDistance;

    float geometricFactor = std::max(0.0f, glm::dot(pointDesc.unitNormal, lightDirection));
    Color irradiance = intensity_ * geometricFactor / (lightDistance * lightDistance);

    Color reflectedLightFactor =
        pointDesc.hitShape->material().reflect(pointDesc.unitNormal, -pointDesc.ray.direction(), lightDirection);

    return irradiance * reflectedLightFactor;
}
} // namespace cg