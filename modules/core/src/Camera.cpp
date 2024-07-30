#include "core/Camera.h"

#include "core/TransformUtils.h"
#include "glm/geometric.hpp"
#include "glm/mat3x3.hpp"

#include <cassert>

namespace cg {
Camera::Camera() { update(); }

void Camera::setViewDirection(const glm::vec3& viewDirection, const glm::vec3& upVector) {
    assert(glm::length(viewDirection) != 0);
    assert(glm::length(upVector) != 0);
    assert(viewDirection != upVector);

    viewDirection_ = viewDirection;
    upVector_ = upVector;
}
void Camera::lookAt(const Point& target, const glm::vec3& upVector) { setViewDirection(target - position(), upVector); }

void Camera::setResolution(Resolution newRes, bool adjustAspectRatio) {
    assert(newRes.width > 0 && newRes.height > 0);
    resolution_ = newRes;
    if (adjustAspectRatio) {
        setAspectRatio(static_cast<float>(resolution_.width) / resolution_.height);
    }
}
void Camera::setAspectRatio(float ratio) {
    assert(ratio > 0.0f);
    float newHeight = viewPlaneSize_.width / ratio;
    setViewPlaneSize({viewPlaneSize_.width, newHeight});
}

void Camera::setViewPlaneSize(Size2d newSize) {
    assert(newSize.width > 0 && newSize.height > 0);
    viewPlaneSize_ = newSize;
}
void Camera::setViewPlaneWidth(float newWidth) {
    assert(newWidth > 0);
    float ar = aspectRatio();
    setViewPlaneSize({newWidth, newWidth / ar});
}

void Camera::setViewPlaneDistance(float distance) {
    assert(distance > 0 && distance < viewLimit_);
    viewPlaneDistance_ = distance;
}

void Camera::setViewLimit(float limit) {
    assert(limit > viewPlaneDistance_);
    viewLimit_ = limit;
}

void Camera::update() {
    pixelSize_ = {viewPlaneSize_.width / resolution().width, viewPlaneSize_.height / resolution().height};

    viewDirection_ = glm::normalize(viewDirection_);
    rightVector_ = glm::normalize(glm::cross(viewDirection_, upVector_));
    pixelRightVector_ = pixelSize_.width * rightVector_;
    upVector_ = glm::normalize(glm::cross(rightVector_, viewDirection_));
    pixelUpVector_ = pixelSize_.height * upVector_;

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

} // namespace cg
