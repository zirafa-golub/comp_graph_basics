#include "gtest/gtest.h"

#include "core/DirectionalLight.h"

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

TEST(DirectionalLightTest, illuminate_validLightAngle_shouldReturnExpected) {
    TestShape shape;
    shape.setMaterial(std::make_unique<TestMaterial>());

    DirectionalLight light({0, -1, 0}, Color(8.0f, 4.0f, 2.0f));

    Ray hitRay{{2, 2, 0}, {-2, -1, 0}};
    HitDesc hit = {&shape, hitRay, 1.0f, {std::sqrt(3) / 2, 0.5f, 0}};

    Color reflectedLight = light.illuminate(hit);
    assertColorsFloatEqual(reflectedLight, Color(4.0f, 2.0f, 1.0f));
}

TEST(DirectionalLightTest, illuminate_invalidLightAngle_shouldReturnZero) {
    TestShape shape;
    shape.setMaterial(std::make_unique<TestMaterial>());

    DirectionalLight light({-1, 0, 0}, Color(8.0f, 4.0f, 2.0f));

    Ray hitRay{{2, 2, 0}, {-2, -1, 0}};
    HitDesc hit = {&shape, hitRay, 1.0f, {0, 1.0f, 0}};

    Color reflectedLight = light.illuminate(hit);
    assertColorsFloatEqual(reflectedLight, Color(0.0f, 0.0f, 0.0f));
}