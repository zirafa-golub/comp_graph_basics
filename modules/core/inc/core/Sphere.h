#pragma once

#include "BasicTypes.h"
#include "Shape.h"

#include <array>
#include <cmath>
#include <expected>

#include "glm/geometric.hpp"

namespace cg {
class Sphere : public Shape {
private:
    struct QuadEquationSolution {
        uint8_t solutionCount;
        std::array<float, 2> solutions;

        QuadEquationSolution() : solutionCount(0) {}
        QuadEquationSolution(float solution) : solutionCount(1), solutions({solution, 0}) {}
        QuadEquationSolution(float solution1, float solution2) : solutionCount(2), solutions({solution1, solution2}) {}
    };

public:
    Sphere(float radius) : radius_(radius) {}

    const Point& center() const;

    float radius() const;

    std::expected<HitDesc, Error> hit(const Ray& ray, float tMin, float tMax) const override;

private:
    HitDesc formHitDesc(const Ray& ray, float tHit) const;

private:
    float radius_;
};
} // namespace cg