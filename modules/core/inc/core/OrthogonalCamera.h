#pragma once

#include "core/Camera.h"

#include "glm/mat4x4.hpp"

namespace cg {
class OrthogonalCamera : public Camera {
public:
    Ray castRay(unsigned pixelX, unsigned pixelY) const override;

    FrustumPoints frustumPoints() const override;

    const glm::mat4& projectionTransform() const override { return projectionTransform_; }

protected:
    void onUpdated() override;

private:
    glm::mat4 projectionTransform_;

    // Ray casting helper
    Point bottomLeftPixel_;
};
} // namespace cg
