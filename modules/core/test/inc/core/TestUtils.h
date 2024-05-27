#pragma once

#include "core/BasicTypes.h"
#include "core/Material.h"
#include "core/Shape.h"

#include "glm/vec3.hpp"

namespace cg {
class Color;
}

void assertColorsFloatEqual(const cg::Color& left, const cg::Color& right);
void assertSize2dFloatEqual(const cg::Size2d& left, const cg::Size2d& right);
