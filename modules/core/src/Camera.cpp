#include "core/Camera.h"

namespace cg {
Camera::Camera() { update(); }

const glm::vec3& Camera::viewDirection() const { return viewDirection_; }
void Camera::setViewDirection(const glm::vec3& viewDirection, const glm::vec3& upVector) {
    assert(glm::length(viewDirection) != 0);
    assert(glm::length(upVector) != 0);
    assert(viewDirection != upVector);

    viewDirection_ = viewDirection;
    upVector_ = upVector;
}

Camera::Resolution Camera::resolution() const { return resolution_; }
void Camera::setResolution(Resolution newRes) { resolution_ = newRes; }

Size2d Camera::viewPlaneSize() const { return viewPlaneSize_; }
void Camera::setViewPlaneSize(Size2d newSize) { viewPlaneSize_ = newSize; }

void Camera::update() {
    viewDirection_ = glm::normalize(viewDirection_);
    rightVector_ = glm::normalize(glm::cross(viewDirection_, upVector_));
    upVector_ = glm::normalize(glm::cross(rightVector_, viewDirection_));
    pixelSize_ = {viewPlaneSize_.width / resolution().width, viewPlaneSize_.height / resolution().height};
}

const glm::vec3& Camera::rightVector() const { return rightVector_; }
const glm::vec3& Camera::upVector() const { return upVector_; }
Size2d Camera::pixelSize() const { return pixelSize_; }
} // namespace cg