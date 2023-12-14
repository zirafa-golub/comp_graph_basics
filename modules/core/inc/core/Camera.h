#pragma once

#include "core/BasicTypes.h"
#include "core/Movable.h"
#include "core/Ray.h"

#include <cassert>

#include "glm/geometric.hpp"

namespace cg {
class Camera : public Movable {
private:
    glm::vec3 viewDirection_ = {1, 0, 0};

public:
    virtual Ray castRay(unsigned pixel_x, unsigned pixel_y) = 0;

    const glm::vec3& viewDirection() const { return viewDirection_; }
    void setViewDirection(const glm::vec3& view_direction) {
        assert(glm::length(view_direction) != 0);
        viewDirection_ = view_direction;
    }
};
} // namespace cg