#pragma once

#include "common/Error.h"
#include "core/BasicTypes.h"
#include "core/Movable.h"
#include "core/Ray.h"

#include <expected>

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
    virtual std::expected<HitDesc, Error> hit(const Ray& ray, float t_start, float t_end) const = 0;
};
} // namespace cg