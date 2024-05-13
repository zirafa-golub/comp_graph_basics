#pragma once

#include "core/Angle.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include <vector>

namespace cg {

using Point = glm::vec3;
using Point2d = glm::vec2;

struct Size2d {
    float width;
    float height;

    Size2d() : width(0.0f), height(0.0f) {}
    Size2d(float width_, float height_) : width(width_), height(height_) {}

    bool operator==(const Size2d&) const = default;
    bool operator!=(const Size2d&) const = default;
};

} // namespace cg
