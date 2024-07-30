#include "core/OrthogonalCamera.h"

#include "core/TransformUtils.h"

#include <cassert>

namespace cg {
Ray OrthogonalCamera::castRay(unsigned pixelX, unsigned pixelY) const {
    assert(pixelX < resolution().width);
    assert(pixelY < resolution().height);

    return Ray(bottomLeftPixel_ + static_cast<float>(pixelX) * pixelRightVector() +
                   static_cast<float>(pixelY) * pixelUpVector(),
               viewDirection());
}

Camera::FrustumPoints OrthogonalCamera::frustumPoints() const {
    FrustumPoints frustum;

    const glm::vec3& cameraPos = position();
    glm::vec3 toNearPlane = viewDirection() * viewPlaneDistance();
    glm::vec3 toFarPlane = viewDirection() * viewLimit();
    glm::vec3 rightOffset = rightVector() * (viewPlaneSize().width / 2);
    glm::vec3 upOffset = upVector() * (viewPlaneSize().height / 2);

    frustum.nlb = cameraPos + toNearPlane - rightOffset - upOffset;
    frustum.nlt = cameraPos + toNearPlane - rightOffset + upOffset;
    frustum.nrb = cameraPos + toNearPlane + rightOffset - upOffset;
    frustum.nrt = cameraPos + toNearPlane + rightOffset + upOffset;
    frustum.flb = cameraPos + toFarPlane - rightOffset - upOffset;
    frustum.flt = cameraPos + toFarPlane - rightOffset + upOffset;
    frustum.frb = cameraPos + toFarPlane + rightOffset - upOffset;
    frustum.frt = cameraPos + toFarPlane + rightOffset + upOffset;

    return frustum;
}

void OrthogonalCamera::onUpdated() {
    Size2d vps = viewPlaneSize();

    // NOTE: Locally, the camera always looks down the -z axis direction
    projectionTransform_ = TransformUtils::viewToCanonicalMatrix(viewPlaneSize(), -viewPlaneDistance(), -viewLimit());

    float left = -viewPlaneSize().width / 2 + 0.5f * pixelSize().width;
    float bottom = -viewPlaneSize().height / 2 + 0.5f * pixelSize().height;
    bottomLeftPixel_ = position() + left * rightVector() + bottom * upVector();
}
} // namespace cg
