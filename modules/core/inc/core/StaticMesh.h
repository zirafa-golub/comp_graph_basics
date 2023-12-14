#pragma once

#include "core/Shape.h"

namespace cg {
class staticMesh : public Shape {
public:
    std::expected<HitDesc, Error> hit(const Ray& ray, float t_start, float t_end) const override;
};
} // namespace cg