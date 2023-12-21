#pragma once

#include "core/Camera.h"

#include <cassert>

namespace cg {
class OrthogonalCamera : public Camera {
public:
    Ray castRay(unsigned pixelX, unsigned pixelY) const override;
};
} // namespace cg