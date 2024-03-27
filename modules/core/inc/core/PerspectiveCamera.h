#pragma once

#include "core/BasicTypes.h"
#include "core/Camera.h"

namespace cg {
class PerspectiveCamera : public Camera {
public:
    Angle fieldOfView() const;
    void setFieldOfView(Angle fov);

    float viewPlaneDistance() const;
    void setViewPlaneDistance(float distance);

    Ray castRay(unsigned pixelX, unsigned pixelY) const override;

private:
    float viewPlaneDistance_ = 20.0f;
};
} // namespace cg
