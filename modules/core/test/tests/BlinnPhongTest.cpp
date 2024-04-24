#include "gtest/gtest.h"

#include "core/BlinnPhong.h"

#include "core/TestUtils.h"

#include "glm/geometric.hpp"

#include <cmath>
#include <numbers>

using namespace cg;

TEST(BlinnPhongTest, reflect_validViewAndLightAngle_shouldReturnExpected) {
    BlinnPhong material(
        Color(0.1f * std::numbers::pi_v<float>, 0.15f * std::numbers::pi_v<float>, 0.2f * std::numbers::pi_v<float>),
        Color(0.1f, 0.2f, 0.3f), 2);

    Color reflectionFactors =
        material.reflect(glm::vec3(0, 1, 0), glm::normalize(glm::vec3(1, 1 / sqrt(3), 0)), glm::vec3(0, 1, 0));

    assertColorsFloatEqual(reflectionFactors, Color(0.175f, 0.3f, 0.425f));
}

TEST(BlinnPhongTest, reflect_invalidAngle_shouldReturnNoSpecular) {
    BlinnPhong material(
        Color(0.1f * std::numbers::pi_v<float>, 0.15f * std::numbers::pi_v<float>, 0.2f * std::numbers::pi_v<float>),
        Color(0.1f, 0.2f, 0.3f), 2);

    Color reflectionFactors =
        material.reflect(glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::normalize(glm::vec3(1, -0.5, 0)));

    assertColorsFloatEqual(reflectionFactors, Color(0.1f, 0.15f, 0.2f));
}
