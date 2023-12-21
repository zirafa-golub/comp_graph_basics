#include "core/OrthogonalCamera.h"

namespace cg {
Ray OrthogonalCamera::castRay(unsigned pixelX, unsigned pixelY) const {
    assert(pixelX < resolution().width);
    assert(pixelY < resolution().height);

    float left = -viewPlaneSize().width / 2;
    float bottom = -viewPlaneSize().height / 2;
    float offsetHor = left + (pixelX + 0.5f) * pixelSize().width;
    float offsetVer = bottom + (pixelY + 0.5f) * pixelSize().height;

    return Ray(position() + offsetHor * rightVector() + offsetVer * upVector(), viewDirection());
}
} // namespace cg