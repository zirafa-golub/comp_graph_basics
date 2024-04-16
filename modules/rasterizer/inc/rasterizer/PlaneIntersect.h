#pragma once

#include "core/BasicTypes.h"

#include "glm/geometric.hpp"
#include "glm/vec3.hpp"

#include <cassert>
#include <optional>

namespace cg {
class PlaneIntersect {
public:
    PlaneIntersect(const Point& a, const Point& b, const Point& c)
        : planePoint_(a), normal_(glm::normalize(glm::cross(b - a, c - a))),
          constant_(-glm::dot(planePoint_, normal_)) {}

    bool isVertexAbove(const Point& p) const { return glm::dot(p, normal_) + constant_ > 0; }

    std::optional<Point> intersectSegment(const Point& a, const Point& b) const {
        float divisor = glm::dot(a - b, normal_);
        if (divisor == 0) {
            return std::nullopt;
        }
        float intersection = (glm::dot(a, normal_) + constant_) / divisor;
        if (intersection < 0.0f || intersection > 1.0f) {
            return std::nullopt;
        }
        return a + intersection * (b - a);
    }

private:
    Point planePoint_;
    glm::vec3 normal_;
    float constant_;
};
} // namespace cg
