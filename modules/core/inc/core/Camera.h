#pragma once

#include "core/BasicTypes.h"
#include "core/Movable.h"
#include "core/Ray.h"

#include <cassert>

#include "glm/geometric.hpp"
#include "glm/vec2.hpp"

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

    class CameraModifier {
    public:
        CameraModifier(Camera& camera) : camera_(camera) {}
        ~CameraModifier() { camera_.update(); }

        CameraModifier& setViewDirection(const glm::vec3& viewDirection, const glm::vec3& upVector) {
            camera_.setViewDirection(viewDirection, upVector);
            return *this;
        }
        CameraModifier& setResolution(Resolution newRes) {
            camera_.setResolution(newRes);
            return *this;
        }
        CameraModifier& setViewPlaneSize(Size2d newSize) {
            camera_.setViewPlaneSize(newSize);
            return *this;
        }

    private:
        Camera& camera_;
    };

    Camera();

    virtual Ray castRay(unsigned pixelX, unsigned pixelY) const = 0;

    const glm::vec3& viewDirection() const;
    const glm::vec3& upVector() const;
    Resolution resolution() const;
    Size2d viewPlaneSize() const;
    Size2d pixelSize() const;

    CameraModifier modify() { return CameraModifier(*this); }

protected:
    const glm::vec3& rightVector() const;

    virtual void update();

private:
    void setViewDirection(const glm::vec3& viewDirection, const glm::vec3& upVector);
    void setResolution(Resolution newRes);
    void setViewPlaneSize(Size2d newSize);

    glm::vec3 viewDirection_ = {1, 0, 0};
    glm::vec3 rightVector_;
    glm::vec3 upVector_ = {0, 1, 0};
    Resolution resolution_ = {100, 100};
    Size2d viewPlaneSize_ = Size2d{1.0f, 1.0f};
    Size2d pixelSize_;
};
} // namespace cg