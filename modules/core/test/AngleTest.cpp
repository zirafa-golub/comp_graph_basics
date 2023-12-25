#include "gtest/gtest.h"

#include "core/Angle.h"

#include <numbers>

using namespace cg;
using namespace cg::angle_literals;

TEST(AngleTest, defaultConstructor_shouldReturnZero) {
    Angle angle;

    EXPECT_EQ(angle.asDeg(), 0);
    EXPECT_EQ(angle.asRad(), 0);
}

TEST(AngleTest, rad_shouldReturnExpectedAngle) {
    auto angle = Angle::rad(std::numbers::pi_v<float>);

    EXPECT_FLOAT_EQ(angle.asDeg(), 180);
    EXPECT_FLOAT_EQ(angle.asRad(), std::numbers::pi_v<float>);
}

TEST(AngleTest, deg_shouldReturnExpectedAngle) {
    auto angle = Angle::deg(180);

    EXPECT_FLOAT_EQ(angle.asDeg(), 180);
    EXPECT_FLOAT_EQ(angle.asRad(), std::numbers::pi_v<float>);
}

TEST(AngleTest, operatorPlus_shouldAddAngles) {
    auto a1 = Angle::deg(45);
    auto a2 = Angle::rad(std::numbers::pi_v<float> / 4);

    EXPECT_FLOAT_EQ((a1 + a2).asDeg(), 90);
}

TEST(AngleTest, operatorPlusAssign_shouldAddAngles) {
    auto a1 = Angle::deg(45);
    auto a2 = a1 += Angle::rad(std::numbers::pi_v<float> / 4);

    EXPECT_EQ(a1, a2);
    EXPECT_FLOAT_EQ(a1.asRad(), std::numbers::pi_v<float> / 2);
}

TEST(AngleTest, operatorMinus_shouldSubtractAngles) {
    auto a1 = Angle::deg(90);
    auto a2 = Angle::rad(std::numbers::pi_v<float> / 4);

    EXPECT_FLOAT_EQ((a1 - a2).asDeg(), 45);
}

TEST(AngleTest, operatorMinusAssign_shouldSubtractAngles) {
    auto a1 = Angle::deg(180);
    auto a2 = a1 -= Angle::rad(std::numbers::pi_v<float> / 2);

    EXPECT_EQ(a1, a2);
    EXPECT_FLOAT_EQ(a1.asRad(), std::numbers::pi_v<float> / 2);
}

TEST(AngleTest, operatorMultiply_integer_shouldMultiplyAngleWithScalar) {
    auto a1 = Angle::deg(45);
    auto a2 = 4 * a1;
    auto a3 = a1 * 4;

    EXPECT_EQ(a2, a3);
    EXPECT_FLOAT_EQ(a2.asRad(), std::numbers::pi_v<float>);
}

TEST(AngleTest, operatorMultiply_floating_shouldMultiplyAngleByScalar) {
    auto a1 = Angle::deg(45);
    auto a2 = 4.0f * a1;
    auto a3 = a1 * 4.0f;

    EXPECT_EQ(a2, a3);
    EXPECT_FLOAT_EQ(a2.asRad(), std::numbers::pi_v<float>);
}

TEST(AngleTest, operatorMultiplyAssign_integer_shouldMultiplyAngleByScalar) {
    auto a1 = Angle::rad(std::numbers::pi_v<float> / 6);
    auto a2 = a1 *= 3;

    EXPECT_EQ(a1, a2);
    EXPECT_FLOAT_EQ(a1.asRad(), std::numbers::pi_v<float> / 2);
}

TEST(AngleTest, operatorMultiplyAssign_floating_shouldMultiplyAngleByScalar) {
    auto a1 = Angle::rad(std::numbers::pi_v<float> / 6);
    auto a2 = a1 *= 3.0f;

    EXPECT_EQ(a1, a2);
    EXPECT_FLOAT_EQ(a1.asRad(), std::numbers::pi_v<float> / 2);
}

TEST(AngleTest, operatorDivide_integer_shouldDivideAngleByScalar) {
    auto a1 = Angle::deg(90);

    EXPECT_FLOAT_EQ((a1 / 2).asRad(), std::numbers::pi_v<float> / 4);
}

TEST(AngleTest, operatorDivide_floating_shouldDivideAngleByScalar) {
    auto a1 = Angle::deg(90);

    EXPECT_FLOAT_EQ((a1 / 2.0f).asRad(), std::numbers::pi_v<float> / 4);
}

TEST(AngleTest, operatorDivideAssign_integer_shouldDivideAngleByScalar) {
    auto a1 = Angle::rad(std::numbers::pi_v<float> / 2);
    auto a2 = a1 /= 3;

    EXPECT_EQ(a1, a2);
    EXPECT_FLOAT_EQ(a1.asRad(), std::numbers::pi_v<float> / 6);
}

TEST(AngleTest, operatorDivideAssign_floating_shouldDivideAngleByScalar) {
    auto a1 = Angle::rad(std::numbers::pi_v<float> / 2);
    auto a2 = a1 /= 3.0f;

    EXPECT_EQ(a1, a2);
    EXPECT_FLOAT_EQ(a1.asRad(), std::numbers::pi_v<float> / 6);
}

TEST(AngleTest, literalRad_float_shouldCreateExpectedAngle) {
    auto angle = std::numbers::pi_v<float> * 1.0_rad;

    EXPECT_FLOAT_EQ(angle.asDeg(), 180);
    EXPECT_FLOAT_EQ(angle.asRad(), std::numbers::pi_v<float>);
}

TEST(AngleTest, literalRad_int_shouldCreateExpectedAngle) {
    auto angle = 0.5 * std::numbers::pi_v<float> * 1_rad;

    EXPECT_FLOAT_EQ(angle.asDeg(), 90);
    EXPECT_FLOAT_EQ(angle.asRad(), std::numbers::pi_v<float> / 2);
}

TEST(AngleTest, literalDeg_float_shouldCreateExpectedAngle) {
    auto angle = 135.0_deg;

    EXPECT_FLOAT_EQ(angle.asDeg(), 135);
    EXPECT_FLOAT_EQ(angle.asRad(), 0.75f * std::numbers::pi_v<float>);
}

TEST(AngleTest, literalDeg_int_shouldCreateExpectedAngle) {
    auto angle = 45_deg;

    EXPECT_FLOAT_EQ(angle.asDeg(), 45);
    EXPECT_FLOAT_EQ(angle.asRad(), std::numbers::pi_v<float> / 4);
}