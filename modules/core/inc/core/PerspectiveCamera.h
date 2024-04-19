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

    FrustumPoints frustumPoints() const;

    Ray castRay(unsigned pixelX, unsigned pixelY) const override;

    const glm::mat4& projectionTransform() const override;

protected:
    void onUpdated() override;

private:
    glm::mat4 projectionTransform_;
};
} // namespace cg
