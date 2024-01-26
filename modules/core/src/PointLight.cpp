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
    return intensity_ * geometricFactor / (lightDistance * lightDistance);
}

Light::DistanceDesc PointLight::distanceFrom(const Point& point) const {
    glm::vec3 distanceVec = position() - point;
    float distance = glm::length(distanceVec);
    return DistanceDesc(distance, distanceVec / distance);
}
} // namespace cg
