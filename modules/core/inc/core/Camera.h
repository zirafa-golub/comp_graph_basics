#pragma once

#include "core/BasicTypes.h"
#include "core/Movable.h"
#include "core/Ray.h"
#include "core/Rotatable.h"

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace cg {
class Camera : public Movable {
public:
    struct Resolution {
        unsigned width;
        unsigned height;

        Resolution(unsigned width_, unsigned height_) : width(width_), height(height_) {}
        bool operator==(const Resolution&) const = default;
        bool operator!=(const Resolution&) const = default;
    };

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

    Camera();
    virtual ~Camera() = default;
    Camera(Camera&&) = default;
    Camera(const Camera&) = default;
    Camera& operator=(Camera&&) = default;
    Camera& operator=(const Camera&) = default;

    virtual Ray castRay(unsigned pixelX, unsigned pixelY) const = 0;

    virtual FrustumPoints frustumPoints() const = 0;

    // View vectors modifiers
    const glm::vec3& viewDirection() const { return viewDirection_; }
    void setViewDirection(const glm::vec3& viewDirection, const glm::vec3& upVector);
    void lookAt(const Point& target, const glm::vec3& upVector);
    const glm::vec3& upVector() const { return upVector_; }

    // View plane, resolution and pixel size modifiers
    Resolution resolution() const { return resolution_; }
    void setResolution(Resolution newRes, bool adjustAspectRatio = true);
    float aspectRatio() const { return viewPlaneSize_.width / viewPlaneSize_.height; }
    void setAspectRatio(float ratio);
    Size2d viewPlaneSize() const { return viewPlaneSize_; }
    void setViewPlaneSize(Size2d newSize);
    void setViewPlaneWidth(float newWidth);
    float viewPlaneDistance() const { return viewPlaneDistance_; }
    void setViewPlaneDistance(float distance);
    float viewLimit() const { return viewLimit_; }
    void setViewLimit(float limit);
    Size2d pixelSize() const { return pixelSize_; }

    // Must be called after modifying the camera, otherwise non-modifying calls might not work correctly
    void update();

    const glm::mat4& cameraTransform() const { return cameraTransform_; }
    virtual const glm::mat4& projectionTransform() const = 0;
    const glm::mat4& viewportTransform() const { return viewportTransform_; }

protected:
    virtual void onUpdated(){};
    const glm::vec3& rightVector() const { return rightVector_; }
    const glm::vec3& pixelRightVector() const { return pixelRightVector_; }
    const glm::vec3& pixelUpVector() const { return pixelUpVector_; }

private:
    static constexpr glm::vec3 defaultViewDirection = glm::vec3(0, 0, -1);
    static constexpr glm::vec3 defaultUpDirection = glm::vec3(0, 1, 0);

    glm::vec3 viewDirection_ = defaultViewDirection;
    glm::vec3 rightVector_;
    glm::vec3 pixelRightVector_;
    glm::vec3 upVector_ = defaultUpDirection;
    glm::vec3 pixelUpVector_;
    glm::mat4 cameraTransform_;
    glm::mat4 viewportTransform_;
    Resolution resolution_ = {100, 100};
    Size2d viewPlaneSize_ = Size2d{1.0f, 1.0f};
    float viewPlaneDistance_ = 1.0f;
    float viewLimit_ = 100'000.0f;
    Size2d pixelSize_;
};
} // namespace cg
