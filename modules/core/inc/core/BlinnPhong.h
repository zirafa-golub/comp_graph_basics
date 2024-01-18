#pragma once

#include "core/Color.h"
#include "core/Material.h"

namespace cg {
class BlinnPhong : public Material {
public:
    BlinnPhong();
    BlinnPhong(const Color& diffuseReflectance, const Color& specularReflectance, unsigned specularFallOffExponent);

    Color reflect(const glm::vec3& normal, const glm::vec3& viewerDirection,
                  const glm::vec3& lightDirection) const override;

    Color diffuseReflectance();
    void setDiffuseReflectance(const Color& newDiffuse);
    Color specularCoefficient();
    void setspecularCoefficient(const Color& newSpecular);
    unsigned specularFallOffExponent();
    void setSpecularFallOffExponent(unsigned newFallOff);

private:
    Color diffuseReflectance_;
    Color specularCoefficient_;
    unsigned specularFallOffExponent_;
};
} // namespace cg
