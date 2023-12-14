#include "common/Math.h"

#include "gtest/gtest.h"

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