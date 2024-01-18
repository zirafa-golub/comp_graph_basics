#pragma once

#include "core/BasicTypes.h"
#include "core/Shape.h"

namespace cg {

class Color;

struct Light {
    virtual ~Light() = default;
    virtual Color illuminate(const HitDesc& pointDesc) const = 0;
};
} // namespace cg