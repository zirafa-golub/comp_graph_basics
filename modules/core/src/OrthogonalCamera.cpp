#include "core/OrthogonalCamera.h"

#include "core/TransformUtils.h"

#include <cassert>

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

const glm::mat4& OrthogonalCamera::projectionTransform() const { return projectionTransform_; }

void OrthogonalCamera::onUpdated() {
    Size2d vps = viewPlaneSize();

    // NOTE: Locally, the camera always looks down the -z axis direction
    projectionTransform_ = TransformUtils::viewToCanonicalMatrix(viewPlaneSize(), -viewPlaneDistance(), -viewLimit());
}
} // namespace cg
