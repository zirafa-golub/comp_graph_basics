#include "core/Camera.h"

#include "core/TransformUtils.h"
#include "glm/geometric.hpp"
#include "glm/mat3x3.hpp"

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
void Camera::lookAt(const Point& target, const glm::vec3& upVector) { setViewDirection(target - position(), upVector); }

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

float Camera::viewPlaneDistance() const { return viewPlaneDistance_; }

void Camera::setViewPlaneDistance(float distance) {
    assert(distance > 0 && distance < viewLimit_);
    viewPlaneDistance_ = distance;
}

float Camera::viewLimit() const { return viewLimit_; }

void Camera::setViewLimit(float limit) {
    assert(limit > viewPlaneDistance_);
    viewLimit_ = limit;
}

void Camera::update() {
    pixelSize_ = {viewPlaneSize_.width / resolution().width, viewPlaneSize_.height / resolution().height};

    viewDirection_ = glm::normalize(viewDirection_);
    rightVector_ = glm::normalize(glm::cross(viewDirection_, upVector_));
    upVector_ = glm::normalize(glm::cross(rightVector_, viewDirection_));

    // NOTE: Locally, the camera always looks down the -z axis direction and canonical view volume is [-1, 1] for all
    // axes
    cameraTransform_ = TransformUtils::rotationToBasisMatrix(rightVector_, upVector_, -viewDirection_) *
                       TransformUtils::translateMatrix(-position());
    viewportTransform_ =
        glm::mat4({resolution_.width / 2.0f, 0, 0, 0}, {0, resolution_.height / 2.0f, 0, 0}, {0, 0, 1, 0},
                  {(resolution_.width - 1) / 2.0f, (resolution_.height - 1) / 2.0f, 0, 1});

    // Let subclasses update as well
    onUpdated();
}

const glm::vec3& Camera::rightVector() const { return rightVector_; }
const glm::vec3& Camera::upVector() const { return upVector_; }
Size2d Camera::pixelSize() const { return pixelSize_; }

const glm::mat4& Camera::cameraTransform() const { return cameraTransform_; }
const glm::mat4& Camera::viewportTransform() const { return viewportTransform_; }

} // namespace cg
