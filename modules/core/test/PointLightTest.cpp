#include "gtest/gtest.h"

#include "core/PointLight.h"

#include "core/Material.h"
#include "core/Shape.h"

#include "TestUtils.h"

#include <cmath>
#include <expected>

using namespace cg;

TEST(PointLightTest, illuminate_validLightAngle_shouldReturnExpected) {
    UnhittableShape shape;
    shape.setMaterial(std::make_unique<PerfectMirrorMaterial>());

    PointLight light(Color(8.0f, 4.0f, 2.0f));
    light.setPosition(0, 3, 0);

    Ray hitRay{{2, 2, 0}, {-2, -1, 0}};
    HitDesc hit = {&shape, hitRay, 1.0f, {std::sqrt(3) / 2, 0.5f, 0}};

    Color reflectedLight = light.illuminate(hit);
    assertColorsFloatEqual(reflectedLight, Color(1.0f, 0.5f, 0.25f));
}

TEST(PointLightTest, illuminate_invalidLightAngle_shouldReturnZero) {
    UnhittableShape shape;
    shape.setMaterial(std::make_unique<PerfectMirrorMaterial>());

    PointLight light(Color(8.0f, 4.0f, 2.0f));
    light.setPosition(3, 0, 0);

    Ray hitRay{{2, 2, 0}, {-2, -1, 0}};
    HitDesc hit = {&shape, hitRay, 1.0f, {0, 1.0f, 0}};

    Color reflectedLight = light.illuminate(hit);
    assertColorsFloatEqual(reflectedLight, Color(0.0f, 0.0f, 0.0f));
}

TEST(PointLightTest, distanceFrom_shouldReturnExpected) {
    PointLight light;
    light.setPosition(4, 5, 1);

    auto distanceDesc = light.distanceFrom(Point(1, 1, 1));
    EXPECT_FLOAT_EQ(distanceDesc.distance, 5.0f);
    areVectorsParallel(distanceDesc.direction, glm::vec3(3, 4, 0));
}
