#include "common/Math.h"
#include "core/BlinnPhong.h"

#include "glm/geometric.hpp"

#include <cmath>
#include <numbers>

namespace cg {

BlinnPhong::BlinnPhong() : diffuseReflectance_(0, 0, 0), specularCoefficient_(0, 0, 0), specularFallOffExponent_(1) {}

BlinnPhong::BlinnPhong(const Color& diffuseReflectance, const Color& specularReflectance,
                       unsigned specularFallOffExponent)
    : diffuseReflectance_(diffuseReflectance), specularCoefficient_(specularReflectance),
      specularFallOffExponent_(specularFallOffExponent) {}

Color BlinnPhong::reflect(const glm::vec3& normal, const glm::vec3& viewerDirection,
                          const glm::vec3& lightDirection) const {
    assert(areFloatsEqualUlps(glm::length(normal), 1.0f));
    assert(areFloatsEqualUlps(glm::length(viewerDirection), 1.0f));
    assert(areFloatsEqualUlps(glm::length(lightDirection), 1.0f));

    Color diffuse = diffuseReflectance_ / std::numbers::pi_v<float>;

    glm::vec3 unitHalfVector = glm::normalize(viewerDirection + lightDirection);
    float normalHalfVectorCos = glm::dot(normal, unitHalfVector);
    Color specular = specularCoefficient_ *
                     std::powf(std::fmax(0.0f, normalHalfVectorCos), static_cast<float>(specularFallOffExponent_));

    return diffuse + specular;
}

Color BlinnPhong::diffuseReflectance() { return diffuseReflectance_; };
void BlinnPhong::setDiffuseReflectance(const Color& newDiffuse) { diffuseReflectance_ = newDiffuse; }
Color BlinnPhong::specularCoefficient() { return specularCoefficient_; }
void BlinnPhong::setspecularCoefficient(const Color& newSpecular) { specularCoefficient_ = newSpecular; }
unsigned BlinnPhong::specularFallOffExponent() { return specularFallOffExponent_; }
void BlinnPhong::setSpecularFallOffExponent(unsigned newFallOff) { specularFallOffExponent_ = newFallOff; }

} // namespace cg
