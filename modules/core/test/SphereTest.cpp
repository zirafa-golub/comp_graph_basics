#include "core/Sphere.h"
#include "TestUtils.h"

#include "glm/geometric.hpp"
#include "gtest/gtest.h"

#include <limits>

using namespace cg;

TEST(SphereTest, center_shouldReturnPosition) {
    Sphere sphere{3};
    Point p{1, 2, 3};
    sphere.setPosition(p);

    EXPECT_EQ(sphere.center(), p);
}

TEST(SphereTest, hit_twoHits_shouldReturnExpected) {
    Sphere sphere{3};
    sphere.setPosition(5, 0, 0);

    Ray ray{{0, 0, 0}, {1, 0, 0}};

    auto result = sphere.hit(ray, 1, 6);

    ASSERT_TRUE(result.has_value());
    auto& hit = result.value();
    EXPECT_EQ(hit.hitShape, &sphere);
    EXPECT_EQ(hit.ray, ray);
    EXPECT_FLOAT_EQ(hit.tHit, 2);
    assertVec3FloatEqual(hit.unitNormal, glm::vec3(-1, 0, 0));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}

TEST(SphereTest, hit_twoHitsOneOutOfRange_shouldReturnExpected) {
    Sphere sphere{4};
    sphere.setPosition(0, 0, 0);

    Ray ray{{0, 0, 0}, {2, 0, 0}};

    auto result = sphere.hit(ray, 1, 6);

    ASSERT_TRUE(result.has_value());
    auto& hit = result.value();
    EXPECT_EQ(hit.hitShape, &sphere);
    EXPECT_EQ(hit.ray, ray);
    EXPECT_EQ(hit.tHit, 2);
    assertVec3FloatEqual(hit.unitNormal, glm::vec3(-1, 0, 0));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}

TEST(SphereTest, hit_oneHit_shouldReturnExpected) {
    Sphere sphere{3};
    sphere.setPosition(4, 0, 0);

    Ray ray{{0, 3, 0}, {1, 0, 0}};

    auto result = sphere.hit(ray, 1, 6);

    ASSERT_TRUE(result.has_value());
    auto& hit = result.value();
    EXPECT_EQ(hit.hitShape, &sphere);
    EXPECT_EQ(hit.ray, ray);
    EXPECT_EQ(hit.tHit, 4);
    assertVec3FloatEqual(hit.unitNormal, glm::vec3(0, 1, 0));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}

TEST(SphereTest, hit_oneHitOutOfRange_shouldReturnError) {
    Sphere sphere{3};
    sphere.setPosition(4, 0, 0);

    Ray ray{{0, 3, 0}, {1, 0, 0}};

    auto result = sphere.hit(ray, 1, 3);

    ASSERT_FALSE(result.has_value());
    auto& error = result.error();
    EXPECT_EQ(error, ErrorCode::NotFound);
}

TEST(SphereTest, hit_miss_shouldReturnError) {
    Sphere sphere{3};
    sphere.setPosition(4, 0, 0);

    Ray ray{{0, 0, 0}, {0, 1, 0}};

    auto result = sphere.hit(ray, 0, std::numeric_limits<float>::infinity());

    ASSERT_FALSE(result.has_value());
    auto& error = result.error();
    EXPECT_EQ(error, ErrorCode::NotFound);
}
