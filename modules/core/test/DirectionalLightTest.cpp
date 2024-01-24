#include "gtest/gtest.h"

#include "core/DirectionalLight.h"

#include "core/Material.h"
#include "core/Shape.h"

#include "TestUtils.h"

#include <cmath>
#include <expected>

using namespace cg;

TEST(DirectionalLightTest, illuminate_validLightAngle_shouldReturnExpected) {
    UnhittableShape shape;
    shape.setMaterial(std::make_unique<PerfectMirrorMaterial>());

    DirectionalLight light({0, -1, 0}, Color(8.0f, 4.0f, 2.0f));

    Ray hitRay{{2, 2, 0}, {-2, -1, 0}};
    HitDesc hit = {&shape, hitRay, 1.0f, {std::sqrt(3) / 2, 0.5f, 0}};

    Color reflectedLight = light.illuminate(hit);
    assertColorsFloatEqual(reflectedLight, Color(4.0f, 2.0f, 1.0f));
}

TEST(DirectionalLightTest, illuminate_invalidLightAngle_shouldReturnZero) {
    UnhittableShape shape;
    shape.setMaterial(std::make_unique<PerfectMirrorMaterial>());

    DirectionalLight light({-1, 0, 0}, Color(8.0f, 4.0f, 2.0f));

    Ray hitRay{{2, 2, 0}, {-2, -1, 0}};
    HitDesc hit = {&shape, hitRay, 1.0f, {0, 1.0f, 0}};

    Color reflectedLight = light.illuminate(hit);
    assertColorsFloatEqual(reflectedLight, Color(0.0f, 0.0f, 0.0f));
}