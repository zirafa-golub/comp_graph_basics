#pragma once

#include "core/Angle.h"
#include "core/Rotation.h"

namespace cg {
class Rotatable {
public:
    const Rotation& rotation() const { return rotation_; }
    void setRotation(const Rotation& rotation) { rotation_ = rotation; }
    void setRotation(Angle x, Angle y, Angle z) { rotation_ = Rotation(x, y, z); }
    void rotate(const Rotation delta) { rotation_ += delta; }
    void rotate(Angle x, Angle y, Angle z) { rotation_ += Rotation(x, y, z); }

private:
    Rotation rotation_ = Rotation{};
};
} // namespace cg
