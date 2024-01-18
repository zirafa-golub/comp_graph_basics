#include "common/math.h"

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

namespace detail {
union FloatIntRep {
    FloatIntRep(float val) : f(val) {}

    bool isNegative() { return i < 0; }

    float f;
    int32_t i;
};
} // namespace detail

bool areFloatsEqualUlps(float left, float right, int32_t maxUlpDistance) {
    detail::FloatIntRep repLeft{left};
    detail::FloatIntRep repRight{right};

    // any comparison with NaN should return false according IEEE standard
    if (std::isnan(left) || std::isnan(right)) {
        return false;
    }

    // +0 and -0 are treated as equal according to IEEE standard
    if (repLeft.isNegative() != repRight.isNegative()) {
        if (left == right) {
            return true;
        }
        return false;
    }

    int32_t ulpDifference = (repLeft.i < repRight.i) ? (repRight.i - repLeft.i) : (repLeft.i - repRight.i);
    return ulpDifference <= maxUlpDistance;
}

bool areFloatsEqualTolerance(float left, float right, float tolerance) { return std::fabs(left - right) <= tolerance; }
} // namespace cg