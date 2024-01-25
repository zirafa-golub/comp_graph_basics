#pragma once

#include "core/BasicTypes.h"

namespace cg {
class Ray {
public:
    Ray(const glm::vec3& origin, const glm::vec3& direction) : origin_(origin), direction_(direction) {}

    Point evaluate(float t) const { return origin_ + t * direction_; }
    const Point& origin() const { return origin_; }
    const glm::vec3& direction() const { return direction_; }

    bool operator==(const Ray& other) const = default;
    bool operator!=(const Ray& other) const = default;

private:
    Point origin_;
    glm::vec3 direction_;
};
} // namespace cg
