#pragma once

#include "core/BasicTypes.h"
#include "core/Movable.h"
#include "core/Ray.h"

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

    Camera();
    virtual ~Camera() = default;
    Camera(Camera&&) = default;
    Camera(const Camera&) = default;
    Camera& operator=(Camera&&) = default;
    Camera& operator=(const Camera&) = default;

    virtual Ray castRay(unsigned pixelX, unsigned pixelY) const = 0;

    // View vectors modifiers
    const glm::vec3& viewDirection() const;
    void setViewDirection(const glm::vec3& viewDirection, const glm::vec3& upVector);
    const glm::vec3& upVector() const;

    // View plane, resolution and pixel size modifiers
    Resolution resolution() const;
    void setResolution(Resolution newRes, bool adjustAspectRatio = true);
    float aspectRatio() const;
    void setAspectRatio(float ratio);
    Size2d viewPlaneSize() const;
    void setViewPlaneSize(Size2d newSize);
    void setViewPlaneWidth(float newWidth);
    Size2d pixelSize() const;

    // Must be called after modifying the camera, otherwise non-modifying calls might not work correctly
    virtual void update();

protected:
    const glm::vec3& rightVector() const;

private:
    glm::vec3 viewDirection_ = {1, 0, 0};
    glm::vec3 rightVector_;
    glm::vec3 upVector_ = {0, 1, 0};
    Resolution resolution_ = {100, 100};
    Size2d viewPlaneSize_ = Size2d{1.0f, 1.0f};
    Size2d pixelSize_;
};
} // namespace cg
