#include "core/AmbientLight.h"
#include "TestUtils.h"

#include "gtest/gtest.h"

using namespace cg;

TEST(AmbientLightTest, illuminate_shouldReturnExpected) {
    UnhittableShape shape;
    shape.setAmbientReflectance(Color(0.4, 0.5, 0.6));
    AmbientLight light(Color(0.5f, 0.2f, 0.7f));
    HitDesc hit(&shape, Ray(Point(0, 0, 0), glm::vec3(1, 0, 0)), 1.0f, glm::vec3(0, 1, 0));

    Color reflectedLight = light.illuminate(hit);

    assertColorsFloatEqual(reflectedLight, Color(0.2f, 0.1f, 0.42f));
}
