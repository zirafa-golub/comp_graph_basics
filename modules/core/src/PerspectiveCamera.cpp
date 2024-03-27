#include "core/PerspectiveCamera.h"

#include <cmath>

namespace cg {
Angle PerspectiveCamera::fieldOfView() const {
    return Angle::rad(atan((viewPlaneSize().width / 2) / viewPlaneDistance_) * 2);
}

void PerspectiveCamera::setFieldOfView(Angle fov) {
    auto viewPlane = viewPlaneSize();
    float viewPlaneRatio = viewPlane.width / viewPlane.height;

    float newWidth = tan(fov.asRad() / 2) * viewPlaneDistance_ * 2;
    float newHeight = newWidth / viewPlaneRatio;

    setViewPlaneSize(Size2d(newWidth, newHeight));
}

float PerspectiveCamera::viewPlaneDistance() const { return viewPlaneDistance_; }
void PerspectiveCamera::setViewPlaneDistance(float distance) {
    assert(distance > 0);
    viewPlaneDistance_ = distance;
}

Ray PerspectiveCamera::castRay(unsigned pixelX, unsigned pixelY) const {
    assert(pixelX < resolution().width);
    assert(pixelY < resolution().height);

    float left = -viewPlaneSize().width / 2;
    float bottom = -viewPlaneSize().height / 2;
    float offsetHor = left + (pixelX + 0.5f) * pixelSize().width;
    float offsetVer = bottom + (pixelY + 0.5f) * pixelSize().height;

    glm::vec3 direction = viewDirection() * viewPlaneDistance_ + offsetHor * rightVector() + offsetVer * upVector();

    return Ray(position() + direction, direction);
}
} // namespace cg
