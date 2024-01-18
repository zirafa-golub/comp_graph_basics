#include "gtest/gtest.h"

#include "core/PointLight.h"

#include "core/Material.h"
#include "core/Shape.h"

#include "TestUtils.h"

#include <cmath>
#include <expected>

using namespace cg;

class TestShape : public Shape {
    std::expected<HitDesc, Error> hit(const Ray& ray, float tMin, float tMax) const override {
        return std::unexpected(ErrorCode::NotFound);
    }
};

class TestMaterial : public Material {
    Color reflect(const glm::vec3& normal, const glm::vec3& viewerDirection,
                  const glm::vec3& lightDirection) const override {
        return Color(1.0, 1.0, 1.0);
    }
};

TEST(PointLightTest, illuminate_validLightAngle_shouldReturnExpected) {
    TestShape shape;
    shape.setMaterial(std::make_unique<TestMaterial>());

    PointLight light(Color(8.0f, 4.0f, 2.0f));
    light.setPosition(0, 3, 0);

    Ray hitRay{{2, 2, 0}, {-2, -1, 0}};
    HitDesc hit = {&shape, hitRay, 1.0f, {std::sqrt(3) / 2, 0.5f, 0}};

    Color reflectedLight = light.illuminate(hit);
    assertColorsFloatEqual(reflectedLight, Color(1.0f, 0.5f, 0.25f));
}

TEST(PointLightTest, illuminate_invalidLightAngle_shouldReturnZero) {
    TestShape shape;
    shape.setMaterial(std::make_unique<TestMaterial>());

    PointLight light(Color(8.0f, 4.0f, 2.0f));
    light.setPosition(3, 0, 0);

    Ray hitRay{{2, 2, 0}, {-2, -1, 0}};
    HitDesc hit = {&shape, hitRay, 1.0f, {0, 1.0f, 0}};

    Color reflectedLight = light.illuminate(hit);
    assertColorsFloatEqual(reflectedLight, Color(0.0f, 0.0f, 0.0f));
}