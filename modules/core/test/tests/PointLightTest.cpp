#include "gtest/gtest.h"

#include "core/PointLight.h"

#include "core/TestUtils.h"
#include "test_utils/Utils.h"

#include <cmath>

using namespace cg;

TEST(PointLightTest, illuminate_validLightAngle_shouldReturnExpected) {
    PointLight light(Color(8.0f, 4.0f, 2.0f));
    light.setPosition(0, 3, 0);

    Color reflectedLight = light.illuminate({0, 1, 0}, {std::sqrt(3) / 2, 0.5f, 0});
    assertColorsFloatEqual(reflectedLight, Color(1.0f, 0.5f, 0.25f));
}

TEST(PointLightTest, illuminate_invalidLightAngle_shouldReturnZero) {
    PointLight light(Color(8.0f, 4.0f, 2.0f));
    light.setPosition(3, 0, 0);

    Color reflectedLight = light.illuminate({0, 1, 0}, {0, 1, 0});
    assertColorsFloatEqual(reflectedLight, Color(0.0f, 0.0f, 0.0f));
}

TEST(PointLightTest, distanceFrom_shouldReturnExpected) {
    PointLight light;
    light.setPosition(4, 5, 1);

    auto distanceDesc = light.distanceFrom(Point(1, 1, 1));
    EXPECT_FLOAT_EQ(distanceDesc.distance, 5.0f);
    glm::vec3 direction(3, 4, 0);
    assertVec3FloatEqual(distanceDesc.unitDirection, glm::normalize(direction));
}
