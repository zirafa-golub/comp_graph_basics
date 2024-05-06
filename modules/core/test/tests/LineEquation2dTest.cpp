#include "core/LineEquation2d.h"

#include "gtest/gtest.h"

using namespace cg;

TEST(LineEquation2dTest, coordConstructor_shouldReturnExpected) {
    LineEquation2d line(3, -2, 1, 4);

    EXPECT_FLOAT_EQ(line.a(), -6);
    EXPECT_FLOAT_EQ(line.b(), -2);
    EXPECT_FLOAT_EQ(line.c(), 14);
}

TEST(LineEquation2dTest, pointsConstructor_shouldReturnExpected) {
    LineEquation2d line({3, -2}, {1, 4});

    EXPECT_FLOAT_EQ(line.a(), -6);
    EXPECT_FLOAT_EQ(line.b(), -2);
    EXPECT_FLOAT_EQ(line.c(), 14);
}

TEST(LineEquation2dTest, eval_pointOnLine_shouldReturnZero) {
    LineEquation2d line(1, 1, 5, 5);

    EXPECT_NEAR(line.eval(4, 4), 0, 1e-10);
    EXPECT_NEAR(line.eval({4, 4}), 0, 1e-10);
}

TEST(LineEquation2dTest, eval_pointOffLine_shouldReturnExpected) {
    LineEquation2d line(3, 2, -4, 1);

    EXPECT_FLOAT_EQ(line.eval(-3, 2), -6);
    EXPECT_FLOAT_EQ(line.eval({-3, 2}), -6);
}
