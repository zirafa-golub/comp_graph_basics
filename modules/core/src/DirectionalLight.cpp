#include "core/DirectionalLight.h"

#include "glm/geometric.hpp"

#include <limits>

namespace cg {
DirectionalLight::DirectionalLight(const glm::vec3& direction, const Color& intensity)
    : direction_(glm::normalize(direction)), intensity_(intensity) {}

const Color& DirectionalLight::intensity() const { return intensity_; }
void DirectionalLight::setIntensity(const Color& intensity) { intensity_ = intensity; }
const glm::vec3& DirectionalLight::direction() const { return direction_; }
void DirectionalLight::setDirection(const glm::vec3& direction) { direction_ = direction; }

Color DirectionalLight::illuminate(const Point& illuminatedPoint, const glm::vec3& unitNormal) const {
    float geometricFactor = std::max(0.0f, glm::dot(unitNormal, -direction_));
    return intensity_ * geometricFactor;
}

Light::DistanceDesc DirectionalLight::distanceFrom(const Point& point) const {
    return DistanceDesc(std::numeric_limits<float>::infinity(), -direction_);
}
} // namespace cg
