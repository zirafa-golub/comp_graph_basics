#pragma once

#include "core/Ray.h"

#include "glm/geometric.hpp"
#include "glm/vec3.hpp"

#include <optional>

namespace cg {
class Shape;
class ShaderGroup;

struct HitDesc {
    const Shape* hitShape;
    Ray ray;
    float rayHitVal;
    glm::vec3 unitNormal;
    glm::vec3 unitViewDirection;

    HitDesc(const Shape* hitShape_, const Ray& ray_, float rayHitVal_, const glm::vec3& unitNormal_)
        : hitShape(hitShape_), ray(ray_), rayHitVal(rayHitVal_), unitNormal(unitNormal_),
          unitViewDirection(glm::normalize(-ray.direction())) {}
};

class HitDetector {
public:
    virtual ~HitDetector() = default;

    virtual void initForFrame() = 0;
    virtual std::optional<HitDesc> hit(const Ray& ray, float rayMin, float rayMax) const = 0;

    void setShaderGroup(const ShaderGroup& shaderGroup) { shaderGroup_ = &shaderGroup; }
    const ShaderGroup& shaderGroup() const { return *shaderGroup_; }

private:
    const ShaderGroup* shaderGroup_;
};
} // namespace cg
