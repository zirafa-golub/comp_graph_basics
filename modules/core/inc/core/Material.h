#pragma once

#include "core/Color.h"

namespace cg {
struct Material {
    virtual ~Material() = default;

    virtual Color reflect(const glm::vec3& normal, const glm::vec3& viewerDirection,
                          const glm::vec3& lightDirection) const = 0;
};
} // namespace cg