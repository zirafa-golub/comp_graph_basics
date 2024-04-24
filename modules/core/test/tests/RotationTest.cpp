#include "gtest/gtest.h"

#include "core/Rotation.h"

#include <numbers>

using namespace cg;
using namespace cg::angle_literals;

TEST(RotationTest, defaultConstructor_shouldReturnZero) {
    Rotation rot;

    EXPECT_EQ(rot.x, 0_rad);
    EXPECT_EQ(rot.y, 0_rad);
    EXPECT_EQ(rot.z, 0_rad);
}

TEST(RotationTest, fromVec3Rad_shouldReturnExpectedAnglesInDegrees) {
    auto rot = Rotation::fromVec3Rad(
        glm::vec3(std::numbers::pi_v<float> / 2, std::numbers::pi_v<float> / 4, std::numbers::pi_v<float> / 6));

    EXPECT_FLOAT_EQ(rot.x.asDeg(), 90);
    EXPECT_FLOAT_EQ(rot.y.asDeg(), 45);
    EXPECT_FLOAT_EQ(rot.z.asDeg(), 30);
}

TEST(RotationTest, toVec3Rad_shouldReturnExpectedAnglesInRadians) {
    Rotation rot(90_deg, 60_deg, 30_deg);
    glm::vec3 rotInRad = rot.toVec3Rad();

    EXPECT_FLOAT_EQ(rotInRad.x, std::numbers::pi_v<float> / 2);
    EXPECT_FLOAT_EQ(rotInRad.y, std::numbers::pi_v<float> / 3);
    EXPECT_FLOAT_EQ(rotInRad.z, std::numbers::pi_v<float> / 6);
}

TEST(RotationTest, operatorPlus_shouldAddRotations) {
    Rotation rot1(45_deg, 60_deg, 30_deg);
    Rotation rot2(90_deg, 45_deg, 60_deg);
    Rotation rotSum = rot1 + rot2;

    EXPECT_FLOAT_EQ(rotSum.x.asDeg(), 135);
    EXPECT_FLOAT_EQ(rotSum.y.asDeg(), 105);
    EXPECT_FLOAT_EQ(rotSum.z.asDeg(), 90);
}

TEST(RotationTest, operatorPlusAssign_shouldAddRotations) {
    Rotation rot1(45_deg, 60_deg, 30_deg);
    Rotation rot2 = rot1 += Rotation(90_deg, 45_deg, 60_deg);

    EXPECT_EQ(rot1, rot2);
    EXPECT_FLOAT_EQ(rot1.x.asDeg(), 135);
    EXPECT_FLOAT_EQ(rot1.y.asDeg(), 105);
    EXPECT_FLOAT_EQ(rot1.z.asDeg(), 90);
}

TEST(RotationTest, operatorMinus_shouldSubtractRotations) {
    Rotation rot1(45_deg, 60_deg, 30_deg);
    Rotation rot2(90_deg, 45_deg, 60_deg);
    Rotation rotSum = rot1 - rot2;

    EXPECT_FLOAT_EQ(rotSum.x.asDeg(), -45);
    EXPECT_FLOAT_EQ(rotSum.y.asDeg(), 15);
    EXPECT_FLOAT_EQ(rotSum.z.asDeg(), -30);
}

TEST(RotationTest, operatorMinusAssign_shouldSubtractRotations) {
    Rotation rot1(45_deg, 60_deg, 30_deg);
    Rotation rot2 = rot1 -= Rotation(90_deg, 45_deg, 60_deg);

    EXPECT_EQ(rot1, rot2);
    EXPECT_FLOAT_EQ(rot1.x.asDeg(), -45);
    EXPECT_FLOAT_EQ(rot1.y.asDeg(), 15);
    EXPECT_FLOAT_EQ(rot1.z.asDeg(), -30);
}

TEST(RotationTest, operatorUnaryMinus_shouldChangeRotationsSign) {
    Rotation rot1(-45_deg, 60_deg, -30_deg);
    auto rot2 = -rot1;

    EXPECT_FLOAT_EQ(rot2.x.asDeg(), 45);
    EXPECT_FLOAT_EQ(rot2.y.asDeg(), -60);
    EXPECT_FLOAT_EQ(rot2.z.asDeg(), 30);
}

TEST(RotationTest, operatorMultiply_integer_shouldMultiplyRotationsWithScalar) {
    auto rot1 = Rotation(45_deg, 60_deg, 30_deg);
    auto rot2 = 3 * rot1;
    auto rot3 = rot1 * 3;

    EXPECT_EQ(rot2, rot3);
    EXPECT_FLOAT_EQ(rot2.x.asDeg(), 135);
    EXPECT_FLOAT_EQ(rot2.y.asDeg(), 180);
    EXPECT_FLOAT_EQ(rot2.z.asDeg(), 90);
}

TEST(RotationTest, operatorMultiply_floating_shouldMultiplyRotationsByScalar) {
    auto rot1 = Rotation(45_deg, 60_deg, 30_deg);
    auto rot2 = 3.0f * rot1;
    auto rot3 = rot1 * 3.0f;

    EXPECT_EQ(rot2, rot3);
    EXPECT_FLOAT_EQ(rot2.x.asDeg(), 135);
    EXPECT_FLOAT_EQ(rot2.y.asDeg(), 180);
    EXPECT_FLOAT_EQ(rot2.z.asDeg(), 90);
}

TEST(RotationTest, operatorMultiplyAssign_integer_shouldMultiplyRotationsByScalar) {
    Rotation rot1(45_deg, 60_deg, 30_deg);
    Rotation rot2 = rot1 *= 3;

    EXPECT_EQ(rot1, rot2);
    EXPECT_FLOAT_EQ(rot1.x.asDeg(), 135);
    EXPECT_FLOAT_EQ(rot1.y.asDeg(), 180);
    EXPECT_FLOAT_EQ(rot1.z.asDeg(), 90);
}

TEST(RotationTest, operatorMultiplyAssign_floating_shouldMultiplyRotationsByScalar) {
    Rotation rot1(45_deg, 60_deg, 30_deg);
    Rotation rot2 = rot1 *= 3.0f;

    EXPECT_EQ(rot1, rot2);
    EXPECT_FLOAT_EQ(rot1.x.asDeg(), 135);
    EXPECT_FLOAT_EQ(rot1.y.asDeg(), 180);
    EXPECT_FLOAT_EQ(rot1.z.asDeg(), 90);
}

TEST(RotationTest, operatorDivide_integer_shouldDivideRotationsByScalar) {
    auto rot1 = Rotation(45_deg, 60_deg, 30_deg);
    auto rot2 = rot1 / 3;

    EXPECT_FLOAT_EQ(rot2.x.asDeg(), 15);
    EXPECT_FLOAT_EQ(rot2.y.asDeg(), 20);
    EXPECT_FLOAT_EQ(rot2.z.asDeg(), 10);
}

TEST(RotationTest, operatorDivide_floating_shouldDivideRotationsByScalar) {
    auto rot1 = Rotation(45_deg, 60_deg, 30_deg);
    auto rot2 = rot1 / 3.0f;

    EXPECT_FLOAT_EQ(rot2.x.asDeg(), 15);
    EXPECT_FLOAT_EQ(rot2.y.asDeg(), 20);
    EXPECT_FLOAT_EQ(rot2.z.asDeg(), 10);
}

TEST(RotationTest, operatorDivideAssign_integer_shouldDivideRotationsByScalar) {
    Rotation rot1(45_deg, 60_deg, 30_deg);
    Rotation rot2 = rot1 /= 3;

    EXPECT_EQ(rot1, rot2);
    EXPECT_FLOAT_EQ(rot1.x.asDeg(), 15);
    EXPECT_FLOAT_EQ(rot1.y.asDeg(), 20);
    EXPECT_FLOAT_EQ(rot1.z.asDeg(), 10);
}

TEST(RotationTest, operatorDivideAssign_floating_shouldDivideRotationsByScalar) {
    Rotation rot1(45_deg, 60_deg, 30_deg);
    Rotation rot2 = rot1 /= 3.0f;

    EXPECT_EQ(rot1, rot2);
    EXPECT_FLOAT_EQ(rot1.x.asDeg(), 15);
    EXPECT_FLOAT_EQ(rot1.y.asDeg(), 20);
    EXPECT_FLOAT_EQ(rot1.z.asDeg(), 10);
}
