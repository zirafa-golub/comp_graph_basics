#include "gtest/gtest.h"

#include "core/PerspectiveCamera.h"
#include "TestUtils.h"

#include <cmath>

using namespace cg;
using namespace cg::angle_literals;

TEST(PerspectiveCameraTest, setFieldOfView_shouldAffectViewPlaneAfterUpdate) {
    PerspectiveCamera camera;

    Size2d initViewPlaneSize = camera.viewPlaneSize();
    float initDistance = camera.viewPlaneDistance();

    Angle fov = 90_deg;
    EXPECT_NE(fov, camera.fieldOfView());

    camera.setFieldOfView(fov);
    camera.update();

    EXPECT_FLOAT_EQ(fov.asRad(), camera.fieldOfView().asRad());
    EXPECT_FLOAT_EQ(initDistance * 2, camera.viewPlaneSize().width);
}

TEST(PerspectiveCameraTest, frustumPoints_shouldReturnExpectedFrustum) {
    PerspectiveCamera camera;

    glm::vec3 camPos = {1, 1, 1};
    float expectedNear = 6.0f;
    float expectedFar = 100.0f;
    camera.setPosition(camPos);
    camera.setViewDirection({0, 0, 2}, {0, 3, 0});
    camera.setViewPlaneDistance(expectedNear);
    camera.setViewLimit(expectedFar);
    camera.setAspectRatio(2.0f);
    camera.setFieldOfView(60_deg);
    camera.update();

    auto frustum = camera.frustumPoints();

    float expectedNearHorOffset = 2 * std::sqrt(3.0f);
    float expectedNearVerOffset = std::sqrt(3.0f);
    assertVec3FloatEqual(frustum.nlb, camPos + glm::vec3(expectedNearHorOffset, -expectedNearVerOffset, expectedNear));
    assertVec3FloatEqual(frustum.nlt, camPos + glm::vec3(expectedNearHorOffset, expectedNearVerOffset, expectedNear));
    assertVec3FloatEqual(frustum.nrb, camPos + glm::vec3(-expectedNearHorOffset, -expectedNearVerOffset, expectedNear));
    assertVec3FloatEqual(frustum.nrt, camPos + glm::vec3(-expectedNearHorOffset, expectedNearVerOffset, expectedNear));

    float expectedFarHorOffset = 100 * std::sqrt(3.0f) / 3;
    float expectedFarVerOffset = 50 * std::sqrt(3.0f) / 3;
    assertVec3FloatEqual(frustum.flb, camPos + glm::vec3(expectedFarHorOffset, -expectedFarVerOffset, expectedFar));
    assertVec3FloatEqual(frustum.flt, camPos + glm::vec3(expectedFarHorOffset, expectedFarVerOffset, expectedFar));
    assertVec3FloatEqual(frustum.frb, camPos + glm::vec3(-expectedFarHorOffset, -expectedFarVerOffset, expectedFar));
    assertVec3FloatEqual(frustum.frt, camPos + glm::vec3(-expectedFarHorOffset, expectedFarVerOffset, expectedFar));
}

TEST(PerspectiveCameraTest, castRay_evenPixelCount_shouldReturnExpectedRay) {
    PerspectiveCamera camera;

    camera.setPosition(1, 2, 3);
    camera.setResolution({100, 100});
    camera.setViewDirection({2, 0, 0}, {0, 3, 0});
    camera.setViewPlaneDistance(50);
    camera.setAspectRatio(1.0f);
    camera.setFieldOfView(90_deg);
    camera.update();

    Ray ray = camera.castRay(10, 12);

    EXPECT_EQ(ray.origin(), Point(51, -35.5f, -36.5f));
    EXPECT_EQ(ray.direction(), glm::vec3(50, -37.5f, -39.5f));
}

TEST(PerspectiveCameraTest, castRay_oddPixelCount_shouldReturnExpectedRay) {
    PerspectiveCamera camera;

    camera.setPosition(5, 4, 2);
    camera.setResolution({99, 99});
    camera.setViewDirection({2, 0, 0}, {0, 3, 0});
    camera.setViewPlaneDistance(99);
    camera.setAspectRatio(1.0f);
    camera.setFieldOfView(90_deg);
    camera.update();

    Ray ray = camera.castRay(52, 63);

    EXPECT_EQ(ray.origin(), Point(104, 32, 8));
    EXPECT_EQ(ray.direction(), glm::vec3(99, 28, 6));
}

#ifndef NDEBUG
TEST(PerspectiveCameraTest, setViewPlaneDistance_zeroDistance_shouldTriggerAssert) {
    PerspectiveCamera camera;

    ASSERT_DEATH(camera.setViewPlaneDistance(0), ".*");
}

TEST(PerspectiveCameraTest, setViewPlaneDistance_distanceBeyondLimit_shouldTriggerAssert) {
    PerspectiveCamera camera;

    ASSERT_DEATH(camera.setViewPlaneDistance(camera.viewLimit() + 1), ".*");
}

TEST(PerspectiveCameraTest, setViewPlaneLimit_limitBeforeDistance_shouldTriggerAssert) {
    PerspectiveCamera camera;

    ASSERT_DEATH(camera.setViewLimit(camera.viewPlaneDistance() / 2), ".*");
}

TEST(PerspectiveCameraTest, castRay_invalidPixel_shouldTriggerAssert) {
    PerspectiveCamera camera;

    camera.setResolution({100, 100});
    camera.update();

    ASSERT_DEATH(camera.castRay(100, 100), ".*");
}
#endif
