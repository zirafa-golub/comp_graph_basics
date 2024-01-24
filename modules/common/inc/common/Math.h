#pragma once

#include <array>
#include <compare>

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

template <typename T>
    requires std::totally_ordered<T>
bool isInRange(T value, T min, T max) {
    return value > min && value < max;
}

template <typename T>
    requires std::totally_ordered<T>
bool isInRangeIncl(T value, T min, T max) {
    return value >= min && value <= max;
}

bool areFloatsEqualUlps(float left, float right, int32_t maxUlpDistance = 4);
bool areFloatsEqualTolerance(float left, float right, float tolerance = 0.000001f);
} // namespace cg
