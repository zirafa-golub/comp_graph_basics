#pragma once

#include <vector>

#include "glm/vec3.hpp"

namespace cg {

using Point = glm::vec3;
using Position = glm::vec3;

struct Size2d {
    float width;
    float height;

    Size2d() : width(0.0f), height(0.0f) {}
    Size2d(float width_, float height_) : width(width_), height(height_) {}

    bool operator==(const Size2d&) const = default;
    bool operator!=(const Size2d&) const = default;
};

} // namespace cg