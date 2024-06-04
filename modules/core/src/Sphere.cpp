#include "core/Angle.h"
#include "core/Mesh.h"
#include "core/Sphere.h"

#include "common/Math.h"

#include "glm/geometric.hpp"

using namespace cg::angle_literals;

namespace cg {
Sphere::Sphere(float radius) : radius_(radius) {}

const Point& Sphere::center() const { return position(); }

float Sphere::radius() const { return radius_; }
} // namespace cg
