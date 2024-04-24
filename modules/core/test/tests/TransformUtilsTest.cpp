#include "core/TransformUtils.h"
#include "test_utils/Utils.h"

#include "gtest/gtest.h"

#include <cmath>
#include <numbers>

using namespace cg;

TEST(TransformUtilsTest, translateMatrix_shouldSetRightColumnOfIdentity) {
    glm::vec3 translation = {2, -1, 4};
    glm::mat4 m = TransformUtils::translateMatrix(translation);
    glm::mat4 expected(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 2, -1, 4, 1);

    assertMat4FloatEqual(m, expected);
}

TEST(TransformUtilsTest, addTranslateLeft_shouldAddToRightColumn) {
    glm::mat4 m =
        glm::identity<glm::mat4>() + glm::mat4(glm::vec4(1), glm::vec4(1), glm::vec4(1), glm::vec4(1, 1, 1, 0));
    glm::vec3 translation = {2, -1, 4};
    glm::mat4 expected(2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 3, 0, 5, 1);

    TransformUtils::addTranslateLeft(m, translation);
    assertMat4FloatEqual(m, expected);
}

TEST(TransformUtilsTest, scaleMatrix_shouldSetDiagonalOfIdentity) {
    glm::vec3 scale = {2, -1, 3};
    glm::mat4 m = TransformUtils::scaleMatrix(scale);
    glm::mat4 expected(2, 0, 0, 0, 0, -1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1);

    assertMat4FloatEqual(m, expected);
}

TEST(TransformUtilsTest, addScaleLeft_shouldMultiplyColumnsByScaleElements) {
    glm::mat4 m = glm::identity<glm::mat4>() + glm::mat4(glm::vec4(1), glm::vec4(1), glm::vec4(1), glm::vec4(1));
    glm::vec3 scale = {2, -1, 3};
    glm::mat4 expected(4, -1, 3, 0, 2, -2, 3, 0, 2, -1, 6, 0, 2, -1, 3, 2);

    TransformUtils::addScaleLeft(m, scale);
    assertMat4FloatEqual(m, expected);
}

TEST(TransformUtilsTest, xyzRotationMatrix_shouldGenerateExpected) {
    Rotation rotation =
        Rotation(Angle::rad(std::numbers::pi_v<float> / 4), Angle::rad(std::numbers::pi_v<float> / 2), Angle::rad(0));
    glm::mat4 m = TransformUtils::xyzRotationMatrix(rotation);
    float sqrt2 = std::sqrt(2.0f);
    glm::mat4 expected(0, 0, -1, 0, sqrt2 / 2, sqrt2 / 2, 0, 0, sqrt2 / 2, -sqrt2 / 2, 0, 0, 0, 0, 0, 1);

    assertMat4EqualTolerance(m, expected);
}

TEST(TransformUtilsTest, addRotationLeftToScaleMatrix_shouldMultiplyColumnsByScaleFactors) {
    glm::mat4 m(2, 0, 0, 0, 0, -1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1);

    Rotation rotation =
        Rotation(Angle::rad(std::numbers::pi_v<float> / 4), Angle::rad(std::numbers::pi_v<float> / 2), Angle::rad(0));
    glm::mat4 rotMat = TransformUtils::xyzRotationMatrix(rotation);

    float sqrt2 = std::sqrt(2.0f);
    glm::mat4 expected(0, 0, -2, 0, -sqrt2 / 2, -sqrt2 / 2, 0, 0, 3 * sqrt2 / 2, -3 * sqrt2 / 2, 0, 0, 0, 0, 0, 1);

    TransformUtils::addRotationLeftToScaleMatrix(m, rotMat);
    assertMat4EqualTolerance(m, expected);
}

TEST(TransformUtilsTest, rotationToBasisMatrix_shouldReturnExpected) {
    glm::mat4 expected = {{-1, 0, 0, 0}, {0, -1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    glm::mat4 result = TransformUtils::rotationToBasisMatrix({-1, 0, 0}, {0, -1, 0}, {0, 0, 1});

    assertMat4FloatEqual(result, expected);
}

TEST(TransformUtilsTest, viewToCanonicalMatrix_shouldReturnExpected) {
    glm::mat4 expected = {{0.1f, 0, 0, 0}, {0, 0.2f, 0, 0}, {0, 0, 0.1f, 0}, {0, 0, 1.5f, 1}};
    glm::mat4 result = TransformUtils::viewToCanonicalMatrix({20, 10}, -5, -25);

    assertMat4FloatEqual(result, expected);
}
