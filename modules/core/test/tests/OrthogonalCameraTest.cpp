#include "core/OrthogonalCamera.h"
#include "test_utils/Utils.h"

#include "gtest/gtest.h"

using namespace cg;

TEST(OrthogonalCameraTest, projectionTransform_shouldReturnExpected) {
    OrthogonalCamera camera;

    camera.setViewPlaneSize({20, 10});
    camera.setViewPlaneDistance(5);
    camera.setViewLimit(25);
    camera.update();

    glm::mat4 expected = {{0.1f, 0, 0, 0}, {0, 0.2f, 0, 0}, {0, 0, 0.1f, 0}, {0, 0, 1.5f, 1}};
    glm::mat4 projectionTransform = camera.projectionTransform();
    assertMat4FloatEqual(projectionTransform, expected);
    assertVec4FloatEqual(projectionTransform * glm::vec4(5, -2, -15, 1), glm::vec4(0.5f, -0.4, 0, 1));
}

TEST(OrthogonalCameraTest, frustumPoints_shouldReturnExpectedFrustum) {
    OrthogonalCamera camera;

    glm::vec3 camPos = {1, 1, 1};
    float expectedNear = 6.0f;
    float expectedFar = 100.0f;
    Size2d viewPlaneSize = {20, 10};

    camera.setPosition(camPos);
    camera.setViewPlaneSize(viewPlaneSize);
    camera.setViewDirection({2, 0, 0}, {0, 3, 0});
    camera.setViewPlaneDistance(expectedNear);
    camera.setViewLimit(expectedFar);
    camera.update();

    auto frustum = camera.frustumPoints();

    float horOffset = viewPlaneSize.width / 2;
    float verOffset = viewPlaneSize.height / 2;
    assertVec3FloatEqual(frustum.nlb, camPos + glm::vec3(expectedNear, -verOffset, -horOffset));
    assertVec3FloatEqual(frustum.nlt, camPos + glm::vec3(expectedNear, verOffset, -horOffset));
    assertVec3FloatEqual(frustum.nrb, camPos + glm::vec3(expectedNear, -verOffset, horOffset));
    assertVec3FloatEqual(frustum.nrt, camPos + glm::vec3(expectedNear, verOffset, horOffset));
    assertVec3FloatEqual(frustum.flb, camPos + glm::vec3(expectedFar, -verOffset, -horOffset));
    assertVec3FloatEqual(frustum.flt, camPos + glm::vec3(expectedFar, verOffset, -horOffset));
    assertVec3FloatEqual(frustum.frb, camPos + glm::vec3(expectedFar, -verOffset, horOffset));
    assertVec3FloatEqual(frustum.frt, camPos + glm::vec3(expectedFar, verOffset, horOffset));
}

TEST(OrthogonalCameraTest, castRay_evenPixelCount_shouldReturnExpectedRay) {
    OrthogonalCamera camera;

    camera.setPosition(1, 2, 3);
    camera.setResolution({100, 100});
    camera.setViewDirection({1, 0, 0}, {0, 1, 0});
    camera.setViewPlaneSize({200, 200});
    camera.update();

    Ray ray = camera.castRay(10, 12);

    EXPECT_EQ(ray.origin(), Point(1, -73, -76));
    EXPECT_EQ(ray.direction(), glm::vec3(1, 0, 0));
}

TEST(OrthogonalCameraTest, castRay_oddPixelCount_shouldReturnExpectedRay) {
    OrthogonalCamera camera;

    camera.setPosition(1, 3, 2);
    camera.setResolution({99, 99});
    camera.setViewDirection({0, 0, 1}, {0, 1, 0});
    camera.setViewPlaneSize({99, 99});
    camera.update();

    Ray ray = camera.castRay(56, 33);

    EXPECT_EQ(ray.origin(), Point(-6, -13, 2));
    EXPECT_EQ(ray.direction(), glm::vec3(0, 0, 1));
}

#ifndef NDEBUG

TEST(OrthogonalCameraTest, castRay_invalidPixel_shouldTriggerAssert) {
    OrthogonalCamera camera;

    camera.setResolution({100, 100});
    camera.update();

    ASSERT_DEATH(camera.castRay(100, 100), ".*");
}

#endif
