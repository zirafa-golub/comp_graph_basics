#include "common/math.h"

#include <cassert>
#include <cmath>

namespace cg {
QuadSolve solveQuadEquation(float a, float b, float c) {
    if (a == 0) {
        if (b != 0) {
            return QuadSolve{-c / b};
        } else {
            return QuadSolve{};
        }
    }

    float discriminant = b * b - 4 * a * c;

    // no solutions
    if (discriminant < 0) {
        return QuadSolve{};
    }

    // one solution
    if (discriminant == 0) {
        return QuadSolve{-b / (2 * a)};
    }

    // two solutions
    float x1, x2;
    float solution_component;
    if (b > 0) {
        solution_component = -b - sqrtf(discriminant);
    } else {
        solution_component = -b + sqrtf(discriminant);
    }
    x1 = (2 * c) / solution_component;
    x2 = solution_component / (2 * a);

    return QuadSolve(x1, x2);
}
} // namespace cg