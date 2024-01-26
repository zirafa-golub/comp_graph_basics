#include "common/Math.h"

#include "gtest/gtest.h"

#include <limits>

using namespace cg;

TEST(MathTest, solveQuadEquation_noSolutions_shouldReturnExpected) {
    QuadSolve solutions = solveQuadEquation(4, 2, 3);

    EXPECT_EQ(solutions.count, 0);
}

TEST(MathTest, solveQuadEquation_oneSolution_shouldReturnExpected) {
    QuadSolve solutions = solveQuadEquation(2, 4, 2);

    EXPECT_EQ(solutions.count, 1);
    EXPECT_EQ(solutions.solutions[0], -1);
}

TEST(MathTest, solveQuadEquation_twoSolutions_shouldReturnExpected) {
    QuadSolve solutions = solveQuadEquation(2, 6, 4);

    EXPECT_EQ(solutions.count, 2);
    EXPECT_EQ(solutions.solutions[0], -2);
    EXPECT_EQ(solutions.solutions[1], -1);
}

TEST(MathTest, solveQuadEquation_aIsZero_shouldReturnExpected) {
    QuadSolve solutions = solveQuadEquation(0, 2, -3);

    EXPECT_EQ(solutions.count, 1);
    EXPECT_EQ(solutions.solutions[0], 1.5f);
}

TEST(MathTest, solveQuadEquation_bIsZero_shouldReturnExpected) {
    QuadSolve solutions = solveQuadEquation(1, 0, -9);

    EXPECT_EQ(solutions.count, 2);
    EXPECT_EQ(solutions.solutions[0], -3);
    EXPECT_EQ(solutions.solutions[1], 3);
}

TEST(MathTest, solveQuadEquation_cIsZero_shouldReturnExpected) {
    QuadSolve solutions = solveQuadEquation(2, -4, 0);

    EXPECT_EQ(solutions.count, 2);
    EXPECT_EQ(solutions.solutions[0], 0);
    EXPECT_EQ(solutions.solutions[1], 2);
}

TEST(MathTest, solveQuadEquation_aAndBAreZero_shouldReturnExpected) {
    QuadSolve solutions = solveQuadEquation(0, 0, 3);

    EXPECT_EQ(solutions.count, 0);
}

TEST(MathTest, solveQuadEquation_aAndCAreZero_shouldReturnExpected) {
    QuadSolve solutions = solveQuadEquation(0, 1, 0);

    EXPECT_EQ(solutions.count, 1);
    EXPECT_EQ(solutions.solutions[0], 0);
}

TEST(MathTest, solveQuadEquation_aBndCAreZero_shouldReturnExpected) {
    QuadSolve solutions = solveQuadEquation(4, 0, 0);

    EXPECT_EQ(solutions.count, 1);
    EXPECT_EQ(solutions.solutions[0], 0);
}

TEST(MathTest, areFloatsEqualUlps_equalFloats_shouldReturnTrue) {
    float a = 1234.0f;
    float b = a;

    EXPECT_TRUE(areFloatsEqualUlps(a, b));
}

TEST(MathTest, areFloatsEqualUlps_positiveClose_shouldReturnTrue) {
    union {
        float f;
        int32_t i;
    } intRep;

    intRep.f = 1234.0f;

    float a = intRep.f;
    intRep.i += 3;
    float b = intRep.f;

    EXPECT_TRUE(areFloatsEqualUlps(a, b));
    EXPECT_TRUE(areFloatsEqualUlps(b, a));
}

TEST(MathTest, areFloatsEqualUlps_negativeClose_shouldReturnTrue) {
    union {
        float f;
        int32_t i;
    } intRep;

    intRep.f = -1234.0f;

    float a = intRep.f;
    intRep.i -= 3;
    float b = intRep.f;

    EXPECT_TRUE(areFloatsEqualUlps(a, b));
    EXPECT_TRUE(areFloatsEqualUlps(b, a));
}

TEST(MathTest, areFloatsEqualUlps_comparisonOfNaNs_shouldReturnFalse) {
    float a = std::numeric_limits<float>::quiet_NaN();
    float b = a;

    EXPECT_FALSE(areFloatsEqualUlps(a, b));
}

TEST(MathTest, areFloatsEqualUlps_comparisonOfDifferentZeroes_shouldReturnTrue) {
    float a = +0.0f;
    float b = -0.0f;

    EXPECT_TRUE(areFloatsEqualUlps(a, b));
    EXPECT_TRUE(areFloatsEqualUlps(b, a));
}

TEST(MathTest, areFloatsEqualUlps_positiveDifferent_shouldReturnFalse) {
    union {
        float f;
        int32_t i;
    } intRep;

    intRep.f = 1234.0f;

    float a = intRep.f;
    intRep.i += 6;
    float b = intRep.f;

    EXPECT_FALSE(areFloatsEqualUlps(a, b));
    EXPECT_FALSE(areFloatsEqualUlps(b, a));
}

TEST(MathTest, areFloatsEqualUlps_negativeDifferent_shouldReturnFalse) {
    union {
        float f;
        int32_t i;
    } intRep;

    intRep.f = -1234.0f;

    float a = intRep.f;
    intRep.i += 6;
    float b = intRep.f;

    EXPECT_FALSE(areFloatsEqualUlps(a, b));
    EXPECT_FALSE(areFloatsEqualUlps(b, a));
}

TEST(MathTest, areFloatsEqualTolerance_equalFloats_shouldReturnTrue) {
    float a = 1234.0f;
    float b = a;

    EXPECT_TRUE(areFloatsEqualTolerance(a, b));
}

TEST(MathTest, areFloatsEqualTolerance_positiveClose_shouldReturnTrue) {
    float a = 0.12345f;
    float b = 0.123455f;

    EXPECT_TRUE(areFloatsEqualTolerance(a, b));
    EXPECT_TRUE(areFloatsEqualTolerance(b, a));
}

TEST(MathTest, areFloatsEqualTolerance_negativeClose_shouldReturnTrue) {
    float a = -0.12345f;
    float b = -0.123455f;

    EXPECT_TRUE(areFloatsEqualTolerance(a, b));
    EXPECT_TRUE(areFloatsEqualTolerance(b, a));
}

TEST(MathTest, areFloatsEqualTolerance_comparisonOfNaNs_shouldReturnFalse) {
    float a = std::numeric_limits<float>::quiet_NaN();
    float b = a;

    EXPECT_FALSE(areFloatsEqualTolerance(a, b));
}

TEST(MathTest, areFloatsEqualTolerance_comparisonOfDifferentZeroes_shouldReturnTrue) {
    float a = +0.0f;
    float b = -0.0f;

    EXPECT_TRUE(areFloatsEqualTolerance(a, b));
    EXPECT_TRUE(areFloatsEqualTolerance(b, a));
}

TEST(MathTest, areFloatsEqualTolerance_positiveDifferent_shouldReturnFalse) {
    float a = 0.1234f;
    float b = 0.123411f;

    EXPECT_FALSE(areFloatsEqualTolerance(a, b));
    EXPECT_FALSE(areFloatsEqualTolerance(b, a));
}

TEST(MathTest, areFloatsEqualTolerance_negativeDifferent_shouldReturnFalse) {
    float a = -0.1234f;
    float b = -0.123411f;

    EXPECT_FALSE(areFloatsEqualTolerance(a, b));
    EXPECT_FALSE(areFloatsEqualTolerance(b, a));
}

TEST(MathTest, isPowerOf2_powerOf2_shouldReturnTrue) { EXPECT_TRUE(isPowerOf2(64)); }
TEST(MathTest, isPowerOf2_one_shouldReturnTrue) { EXPECT_TRUE(isPowerOf2(1)); }
TEST(MathTest, isPowerOf2_notPowerOf2_shouldReturnFalse) { EXPECT_FALSE(isPowerOf2(5)); }

TEST(MathTest, pow2_one_shouldReturnVal) { EXPECT_EQ(pow2(10, 1), 10); }
TEST(MathTest, pow2_powerOf2Exp_shouldReturnVal) { EXPECT_EQ(pow2(10, 4), 10'000); }
#ifndef NDEBUG
TEST(MathTest, pow2_notPowerOf2Exp_shouldTriggerAssert) { ASSERT_DEATH(pow2(10, 5), ".*"); }
TEST(MathTest, pow2_zero_shouldTriggerAssert) { ASSERT_DEATH(pow2(10, 0), ".*"); }
#endif
