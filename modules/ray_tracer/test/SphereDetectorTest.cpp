#include "core/Sphere.h"
#include "hit/SphereHitDetector.h"
#include "test_utils/Utils.h"

#include "gtest/gtest.h"

using namespace cg;

class TestShaderGroup : public ShaderGroup {};

TEST(SphereDetectorTest, hit_twoHits_shouldReturnExpected) {
    Sphere sphere{3};
    sphere.setPosition(5, 0, 0);
    sphere.update();

    Ray ray{{0, 0, 0}, {1, 0, 0}};
    TestShaderGroup shaderGroup;
    shaderGroup.setShape(sphere);
    SphereHitDetector hitDetector;
    hitDetector.setShaderGroup(shaderGroup);

    hitDetector.initForFrame();
    auto result = hitDetector.hit(ray, 1, 6);

    ASSERT_TRUE(result.has_value());
    auto& hit = result.value();
    EXPECT_EQ(hit.hitShape, &sphere);
    EXPECT_EQ(hit.ray, ray);
    EXPECT_FLOAT_EQ(hit.rayHitVal, 2);
    assertVec3FloatEqual(hit.unitNormal, glm::vec3(-1, 0, 0));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}

TEST(SphereDetectorTest, hit_twoHitsOneOutOfRange_shouldReturnExpected) {
    Sphere sphere{4};
    sphere.setPosition(0, 0, 0);
    sphere.update();

    Ray ray{{0, 0, 0}, {2, 0, 0}};
    TestShaderGroup shaderGroup;
    shaderGroup.setShape(sphere);
    SphereHitDetector hitDetector;
    hitDetector.setShaderGroup(shaderGroup);

    hitDetector.initForFrame();
    auto result = hitDetector.hit(ray, 1, 6);

    ASSERT_TRUE(result.has_value());
    auto& hit = result.value();
    EXPECT_EQ(hit.hitShape, &sphere);
    EXPECT_EQ(hit.ray, ray);
    EXPECT_EQ(hit.rayHitVal, 2);
    assertVec3FloatEqual(hit.unitNormal, glm::vec3(-1, 0, 0));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}

TEST(SphereDetectorTest, hit_oneHit_shouldReturnExpected) {
    Sphere sphere{3};
    sphere.setPosition(4, 0, 0);
    sphere.update();

    Ray ray{{0, 3, 0}, {1, 0, 0}};
    TestShaderGroup shaderGroup;
    shaderGroup.setShape(sphere);
    SphereHitDetector hitDetector;
    hitDetector.setShaderGroup(shaderGroup);

    hitDetector.initForFrame();
    auto result = hitDetector.hit(ray, 1, 6);

    ASSERT_TRUE(result.has_value());
    auto& hit = result.value();
    EXPECT_EQ(hit.hitShape, &sphere);
    EXPECT_EQ(hit.ray, ray);
    EXPECT_EQ(hit.rayHitVal, 4);
    assertVec3FloatEqual(hit.unitNormal, glm::vec3(0, 1, 0));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}

TEST(SphereDetectorTest, hit_oneHitOutOfRange_shouldReturnError) {
    Sphere sphere{3};
    sphere.setPosition(4, 0, 0);
    sphere.update();

    Ray ray{{0, 3, 0}, {1, 0, 0}};
    TestShaderGroup shaderGroup;
    shaderGroup.setShape(sphere);
    SphereHitDetector hitDetector;
    hitDetector.setShaderGroup(shaderGroup);

    hitDetector.initForFrame();
    auto result = hitDetector.hit(ray, 1, 3);

    ASSERT_FALSE(result.has_value());
}

TEST(SphereDetectorTest, hit_miss_shouldReturnError) {
    Sphere sphere{3};
    sphere.setPosition(4, 0, 0);
    sphere.update();

    Ray ray{{0, 0, 0}, {0, 1, 0}};
    TestShaderGroup shaderGroup;
    shaderGroup.setShape(sphere);
    SphereHitDetector hitDetector;
    hitDetector.setShaderGroup(shaderGroup);

    hitDetector.initForFrame();
    auto result = hitDetector.hit(ray, 0, std::numeric_limits<float>::infinity());

    ASSERT_FALSE(result.has_value());
}
