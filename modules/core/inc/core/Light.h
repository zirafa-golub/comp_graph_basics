#pragma once

#include "core/BasicTypes.h"
#include "core/Shape.h"

namespace cg {

class Color;

struct Light {
    struct DistanceDesc {
        float distance;
        glm::vec3 unitDirection;
    };

    virtual ~Light() = default;
    virtual Color illuminate(const Point& illuminatedPoint, const glm::vec3& unitNormal) const = 0;
    virtual DistanceDesc distanceFrom(const Point& point) const = 0;
};
} // namespace cg
