#pragma once

#include "core/BasicTypes.h"
#include "core/Camera.h"

namespace cg {
class PerspectiveCamera : public Camera {
public:
    Angle fieldOfView() const;
    void setFieldOfView(Angle fov);

    Ray castRay(unsigned pixelX, unsigned pixelY) const override;

    FrustumPoints frustumPoints() const override;

    const glm::mat4& projectionTransform() const override;

protected:
    void onUpdated() override;

private:
    glm::mat4 projectionTransform_;
};
} // namespace cg
