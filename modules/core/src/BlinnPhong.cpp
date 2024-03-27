#include "common/Math.h"
#include "core/BlinnPhong.h"

#include "glm/geometric.hpp"

#include <cassert>
#include <cmath>
#include <numbers>

namespace cg {

BlinnPhong::BlinnPhong() : BlinnPhong(Color::black(), Color::black(), 1) {}

BlinnPhong::BlinnPhong(const Color& diffuseReflectance, const Color& specularReflectance,
                       unsigned specularFallOffExponent, const Color& surfaceReflectance)
    : diffuseReflectance_(diffuseReflectance), specularCoefficient_(specularReflectance),
      specularFallOffExponent_(specularFallOffExponent), surfaceReflectance_(surfaceReflectance) {
    assert(specularFallOffExponent_ >= 1 && "Specular fall off exponent must be at least 1.");
    assert(isPowerOf2(specularFallOffExponent_) && "Specular fall off exponent must be power of 2.");
}

Color BlinnPhong::reflect(const glm::vec3& normal, const glm::vec3& viewerDirection,
                          const glm::vec3& lightDirection) const {
    assert(areFloatsEqualTolerance(glm::length(normal), 1.0f, 0.0005f));
    assert(areFloatsEqualTolerance(glm::length(viewerDirection), 1.0f, 0.0005f));
    assert(areFloatsEqualTolerance(glm::length(lightDirection), 1.0f, 0.0005f));

    Color diffuse = diffuseReflectance_ / std::numbers::pi_v<float>;

    glm::vec3 reflectionNormal = glm::normalize(viewerDirection + lightDirection);
    float normalReflectionCos = glm::dot(normal, reflectionNormal);
    Color specular = specularCoefficient_ * pow2(std::fmax(0.0f, normalReflectionCos), specularFallOffExponent_);

    return diffuse + specular;
}

Color BlinnPhong::surfaceReflectance() const { return surfaceReflectance_; }
void BlinnPhong::surfaceReflectance(const Color& newReflectance) { surfaceReflectance_ = newReflectance; }
Color BlinnPhong::diffuseReflectance() { return diffuseReflectance_; };
void BlinnPhong::setDiffuseReflectance(const Color& newDiffuse) { diffuseReflectance_ = newDiffuse; }
Color BlinnPhong::specularCoefficient() { return specularCoefficient_; }
void BlinnPhong::setspecularCoefficient(const Color& newSpecular) { specularCoefficient_ = newSpecular; }
unsigned BlinnPhong::specularFallOffExponent() { return specularFallOffExponent_; }
void BlinnPhong::setSpecularFallOffExponent(unsigned newFallOff) {
    assert(newFallOff >= 1 && "Specular fall off exponent must be at least 1.");
    specularFallOffExponent_ = newFallOff;
}

} // namespace cg
