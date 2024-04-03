#pragma once

#include "core/BasicTypes.h"

namespace cg {
class Movable {
public:
    virtual ~Movable() = default;

    const Point& position() const { return position_; }
    void setPosition(const Point& point) { position_ = point; }
    void setPosition(float x, float y, float z) { position_ = {x, y, z}; }
    void translate(const glm::vec3& delta) { position_ += delta; }
    void translate(float x, float y, float z) { position_ += glm::vec3(x, y, z); }

private:
    Point position_ = {0, 0, 0};
};
} // namespace cg
