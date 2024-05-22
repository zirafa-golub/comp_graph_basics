#include "core/PointLight.h"

#include "glm/geometric.hpp"

#include <numbers>

namespace cg {
PointLight::PointLight(const Color& intensity) : intensity_(intensity) {}

const Color& PointLight::intensity() { return intensity_; }
void PointLight::setIntensity(const Color& intensity) { intensity_ = intensity; }

Color PointLight::illuminate(const Point& illuminatedPoint, const glm::vec3& unitNormal) const {
    glm::vec3 pointToLight = position() - illuminatedPoint;
    float lightDistance = glm::length(pointToLight);
    glm::vec3 lightDirection = pointToLight / lightDistance;

    float geometricFactor = std::max(0.0f, glm::dot(unitNormal, lightDirection));
    return intensity_ * geometricFactor / (lightDistance * lightDistance);
}

Light::DistanceDesc PointLight::distanceFrom(const Point& point) const {
    glm::vec3 distanceVec = position() - point;
    float distance = glm::length(distanceVec);
    return DistanceDesc(distance, distanceVec / distance);
}
} // namespace cg
