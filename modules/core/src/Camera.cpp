#include "core/Camera.h"

#include "glm/geometric.hpp"

#include <cassert>

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
void Camera::setResolution(Resolution newRes, bool adjustAspectRatio) {
    assert(newRes.width > 0 && newRes.height > 0);
    resolution_ = newRes;
    if (adjustAspectRatio) {
        setAspectRatio(static_cast<float>(resolution_.width) / resolution_.height);
    }
}

float Camera::aspectRatio() const { return viewPlaneSize_.width / viewPlaneSize_.height; }
void Camera::setAspectRatio(float ratio) {
    assert(ratio > 0.0f);
    float newHeight = viewPlaneSize_.width / ratio;
    setViewPlaneSize({viewPlaneSize_.width, newHeight});
}

Size2d Camera::viewPlaneSize() const { return viewPlaneSize_; }
void Camera::setViewPlaneSize(Size2d newSize) {
    assert(newSize.width > 0 && newSize.height > 0);
    viewPlaneSize_ = newSize;
}
void Camera::setViewPlaneWidth(float newWidth) {
    assert(newWidth > 0);
    float ar = aspectRatio();
    setViewPlaneSize({newWidth, newWidth / ar});
}

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
