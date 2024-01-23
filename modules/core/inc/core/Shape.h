#pragma once

#include "common/Error.h"
#include "core/BasicTypes.h"
#include "core/Material.h"
#include "core/Movable.h"
#include "core/Ray.h"

#include "glm/geometric.hpp"

#include <expected>
#include <memory>

namespace cg {
class Shape;

struct HitDesc {
    const Shape* hitShape;
    Ray ray;
    float tHit;
    glm::vec3 unitNormal;
    glm::vec3 unitViewDirection;

    HitDesc(const Shape* hitShape_, const Ray& ray_, float tHit_, const glm::vec3& unitNormal_)
        : hitShape(hitShape_), ray(ray_), tHit(tHit_), unitNormal(unitNormal_),
          unitViewDirection(glm::normalize(-ray.direction())) {}
};

class Shape : public Movable {
public:
    virtual std::expected<HitDesc, Error> hit(const Ray& ray, float tMin, float tMax) const = 0;

    const Material& material() const { return *material_; }
    Material& material() { return *material_; }
    void setMaterial(std::unique_ptr<Material> material) { material_ = std::move(material); }

private:
    std::unique_ptr<Material> material_;
};
} // namespace cg