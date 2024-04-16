#pragma once

#include "core/BasicTypes.h"
#include "core/Camera.h"

namespace cg {
class PerspectiveCamera : public Camera {
public:
    struct FrustumPoints {
        /*  n -> near (view plane distance)
         *  f -> far (view limit)
         *  l -> left
         *  r -> right
         *  t -> top
         *  b -> bottom
         */
        Point nlb;
        Point nrb;
        Point nlt;
        Point nrt;
        Point flb;
        Point frb;
        Point flt;
        Point frt;
    };

    Angle fieldOfView() const;
    void setFieldOfView(Angle fov);

    float viewPlaneDistance() const;
    void setViewPlaneDistance(float distance);

    float viewLimit() const;
    void setViewLimit(float limit);

    FrustumPoints frustumPoints() const;

    Ray castRay(unsigned pixelX, unsigned pixelY) const override;

private:
    float viewPlaneDistance_ = 1.0f;
    float viewLimit_ = 100'000.0f;
};
} // namespace cg
