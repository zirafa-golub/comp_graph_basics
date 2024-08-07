#include "core/PerspectiveCamera.h"

#include "core/TransformUtils.h"

#include <cmath>

namespace cg {
Angle PerspectiveCamera::fieldOfView() const {
    return Angle::rad(atan((viewPlaneSize().width / 2) / viewPlaneDistance()) * 2);
}

void PerspectiveCamera::setFieldOfView(Angle fov) {
    auto viewPlane = viewPlaneSize();
    float viewPlaneRatio = viewPlane.width / viewPlane.height;

    float newWidth = tan(fov.asRad() / 2) * viewPlaneDistance() * 2;

    setViewPlaneWidth(newWidth);
}

Camera::FrustumPoints PerspectiveCamera::frustumPoints() const {
    FrustumPoints frustum;

    const glm::vec3& cameraPos = position();
    glm::vec3 toNearPlane = viewDirection() * viewPlaneDistance();
    glm::vec3 nearRightOffset = rightVector() * (viewPlaneSize().width / 2);
    glm::vec3 nearUpOffset = upVector() * (viewPlaneSize().height / 2);

    frustum.nlb = cameraPos + toNearPlane - nearRightOffset - nearUpOffset;
    frustum.nlt = cameraPos + toNearPlane - nearRightOffset + nearUpOffset;
    frustum.nrb = cameraPos + toNearPlane + nearRightOffset - nearUpOffset;
    frustum.nrt = cameraPos + toNearPlane + nearRightOffset + nearUpOffset;

    glm::vec3 toFarPlane = viewDirection() * viewLimit();
    float nearFarRatio = viewLimit() / viewPlaneDistance();
    float farPlaneHalfWidth = nearFarRatio * (viewPlaneSize().width / 2);
    float farPlaneHalfHeight = nearFarRatio * (viewPlaneSize().height / 2);
    glm::vec3 farRightOffset = rightVector() * farPlaneHalfWidth;
    glm::vec3 farUpOffset = upVector() * farPlaneHalfHeight;

    frustum.flb = cameraPos + toFarPlane - farRightOffset - farUpOffset;
    frustum.flt = cameraPos + toFarPlane - farRightOffset + farUpOffset;
    frustum.frb = cameraPos + toFarPlane + farRightOffset - farUpOffset;
    frustum.frt = cameraPos + toFarPlane + farRightOffset + farUpOffset;

    return frustum;
}

Ray PerspectiveCamera::castRay(unsigned pixelX, unsigned pixelY) const {
    assert(pixelX < resolution().width);
    assert(pixelY < resolution().height);

    glm::vec3 direction = posToBottomLeftPixel_ + static_cast<float>(pixelX) * pixelRightVector() +
                          static_cast<float>(pixelY) * pixelUpVector();

    return Ray(position() + direction, direction);
}

void PerspectiveCamera::onUpdated() {
    // NOTE: Locally, the camera always looks down the -z axis direction
    float near = -viewPlaneDistance();
    float far = -viewLimit();

    auto viewToCanonical = TransformUtils::viewToCanonicalMatrix(viewPlaneSize(), -viewPlaneDistance(), -viewLimit());
    projectionTransform_ =
        viewToCanonical * glm::mat4({near, 0, 0, 0}, {0, near, 0, 0}, {0, 0, near + far, 1}, {0, 0, -near * far, 0});

    float left = -viewPlaneSize().width / 2 + 0.5f * pixelSize().width;
    float bottom = -viewPlaneSize().height / 2 + 0.5f * pixelSize().height;
    Point bottomLeftPixel = left * rightVector() + bottom * upVector();
    posToBottomLeftPixel_ = viewDirection() * viewPlaneDistance() + bottomLeftPixel;
}
} // namespace cg
