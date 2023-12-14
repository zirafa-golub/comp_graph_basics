#pragma once

#include <array>

namespace cg {
struct QuadSolve {
    QuadSolve() : count(0), solutions({0, 0}) {}
    explicit QuadSolve(float x) : count(1), solutions({x, 0}) {}
    QuadSolve(float x1, float x2)
        : count(2), solutions(x1 < x2 ? std::array<float, 2>{x1, x2} : std::array<float, 2>{x2, x1}) {}

    uint8_t count;
    std::array<float, 2> solutions;
};

QuadSolve solveQuadEquation(float a, float b, float c);
} // namespace cg