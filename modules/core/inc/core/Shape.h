#pragma once

#include "common/Error.h"
#include "core/BasicTypes.h"
#include "core/Material.h"
#include "core/Movable.h"
#include "core/Ray.h"

#include <expected>
#include <memory>

namespace cg {
class Shape;

struct HitDesc {
    const Shape* hitShape;
    Ray ray;
    float tHit;
    glm::vec3 unitNormal;
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