#pragma once

#include "core/Camera.h"

#include "glm/mat4x4.hpp"

namespace cg {
class OrthogonalCamera : public Camera {
public:
    Ray castRay(unsigned pixelX, unsigned pixelY) const override;

    const glm::mat4& projectionTransform() const override;

protected:
    void onUpdated() override;

private:
    glm::mat4 projectionTransform_;
};
} // namespace cg
