#pragma once

#include "core/BasicTypes.h"
#include "core/MeshData.h"
#include "core/Shape.h"

#include "glm/mat4x4.hpp"

#include <array>
#include <cmath>

namespace cg {
class Angle;
class Sphere : public Shape {
public:
    Sphere(float radius);

    const Point& center() const;

    float radius() const;

private:
    float radius_;
};
} // namespace cg
