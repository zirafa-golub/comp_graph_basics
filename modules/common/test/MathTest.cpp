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

TEST(MathTest, areFloatsAlmostEqual_equalFloats_shouldReturnTrue) {
    float a = 1234.0f;
    float b = a;

    EXPECT_TRUE(areFloatsEqualUlps(a, b));
}

TEST(MathTest, areFloatsAlmostEqual_positiveClose_shouldReturnTrue) {
    union {
        float f;
        int32_t i;
    } intRep;

    intRep.f = 1234.0f;

    float a = intRep.f;
    intRep.i += 3;
    float b = intRep.f;

    EXPECT_TRUE(areFloatsEqualUlps(a, b));
}

TEST(MathTest, areFloatsAlmostEqual_negativeClose_shouldReturnTrue) {
    union {
        float f;
        int32_t i;
    } intRep;

    intRep.f = -1234.0f;

    float a = intRep.f;
    intRep.i -= 3;
    float b = intRep.f;

    EXPECT_TRUE(areFloatsEqualUlps(a, b));
}

TEST(MathTest, areFloatsAlmostEqual_comparisonOfNaNs_shouldReturnFalse) {
    float a = std::numeric_limits<float>::quiet_NaN();
    float b = a;

    EXPECT_FALSE(areFloatsEqualUlps(a, b));
}

TEST(MathTest, areFloatsAlmostEqual_comparisonOfDifferentZeroes_shouldReturnFalse) {
    float a = +0.0f;
    float b = -0.0f;

    EXPECT_TRUE(areFloatsEqualUlps(a, b));
}

TEST(MathTest, areFloatsAlmostEqual_positiveCloseSPecifiedDifference_shouldReturnTrue) {
    union {
        float f;
        int32_t i;
    } intRep;

    intRep.f = 1234.0f;

    float a = intRep.f;
    intRep.i += 6;
    float b = intRep.f;

    EXPECT_TRUE(areFloatsEqualUlps(a, b, 6));
}

TEST(MathTest, areFloatsAlmostEqual_positiveDifferent_shouldReturnFalse) {
    union {
        float f;
        int32_t i;
    } intRep;

    intRep.f = 1234.0f;

    float a = intRep.f;
    intRep.i += 6;
    float b = intRep.f;

    EXPECT_FALSE(areFloatsEqualUlps(a, b));
}

TEST(MathTest, areFloatsAlmostEqual_negativeDifferent_shouldReturnFalse) {
    union {
        float f;
        int32_t i;
    } intRep;

    intRep.f = 1234.0f;

    float a = intRep.f;
    intRep.i -= 6;
    float b = intRep.f;

    EXPECT_FALSE(areFloatsEqualUlps(a, b));
}