#include "gtest/gtest.h"

#include "core/Camera.h"
#include "core/TestUtils.h"
#include "test_utils/Utils.h"

using namespace cg;

struct TestCamera : public Camera {
    Ray castRay(unsigned pixelX, unsigned pixelY) const override { return Ray({0, 0, 0}, {0, 0, 1}); }
    const glm::vec3& rightVector() { return Camera::rightVector(); }
    const glm::vec3& pixelRightVector() { return Camera::pixelRightVector(); }
    const glm::vec3& pixelUpVector() { return Camera::pixelUpVector(); }
    const glm::mat4& projectionTransform() const override { return projectionMatrix_; }
    FrustumPoints frustumPoints() const override { return FrustumPoints{}; }

    unsigned onUpdatedCallCount = 0;
    glm::mat4 projectionMatrix_ = glm::identity<glm::mat4>();

protected:
    void onUpdated() override { ++onUpdatedCallCount; }
};

TEST(CameraTest, modify_shouldApplyModificationAfterUpdate) {
    TestCamera camera;

    Size2d initPixelSize = camera.pixelSize();

    Camera::Resolution newRes{200, 300};
    glm::vec3 newDir = {-2, 0, 0};
    glm::vec3 newUp = {1, 0, 1};
    Size2d initViewPlane = camera.viewPlaneSize();
    Size2d newViewPlane{initViewPlane.width / 2, initViewPlane.height / 3};

    camera.setResolution(newRes);
    camera.setViewDirection(newDir, newUp);
    camera.setViewPlaneSize(newViewPlane);

    camera.update();

    Size2d expectedPixelSize(initPixelSize.width / 4, initPixelSize.height / 9);

    EXPECT_EQ(camera.resolution(), newRes);
    EXPECT_EQ(camera.viewPlaneSize(), newViewPlane);
    assertVec3FloatEqual(camera.viewDirection(), glm::vec3(-1, 0, 0));
    assertVec3FloatEqual(camera.rightVector(), glm::vec3({0, 1, 0}));
    assertVec3FloatEqual(camera.pixelRightVector(), glm::vec3({0, expectedPixelSize.width, 0}));
    assertVec3FloatEqual(camera.upVector(), glm::vec3({0, 0, 1}));
    assertVec3FloatEqual(camera.pixelUpVector(), glm::vec3({0, 0, expectedPixelSize.height}));
    EXPECT_EQ(camera.pixelSize(), expectedPixelSize);
}

TEST(CameraTest, setViewDirection_shouldUpdateViewVectorsAfterUpdate) {
    TestCamera camera;

    camera.setViewDirection({-2, 0, 0}, {1, 0, 1});
    camera.update();

    assertVec3FloatEqual(camera.viewDirection(), glm::vec3(-1, 0, 0));
    assertVec3FloatEqual(camera.upVector(), glm::vec3(0, 0, 1));
    assertVec3FloatEqual(camera.rightVector(), glm::vec3(0, 1, 0));
}

TEST(CameraTest, lookAt_shouldUpdateViewVectorsAfterUpdate) {
    TestCamera camera;

    camera.setPosition(1, 0, 0);
    camera.lookAt({1, 0, 5}, {0, 4, 3});
    camera.update();

    assertVec3FloatEqual(camera.viewDirection(), glm::vec3(0, 0, 1));
    assertVec3FloatEqual(camera.upVector(), glm::vec3(0, 1, 0));
    assertVec3FloatEqual(camera.rightVector(), glm::vec3(-1, 0, 0));
}

TEST(CameraTest, setResolution_adjustAspectRatio_shouldModifyViewPlaneSize) {
    TestCamera camera;

    Camera::Resolution initRes = camera.resolution();
    Size2d initPixelSize = camera.pixelSize();
    Size2d initViewPlaneSize = camera.viewPlaneSize();
    Camera::Resolution newRes{initRes.width, initRes.height * 2};
    camera.setResolution(newRes, true);
    camera.update();

    EXPECT_EQ(camera.resolution(), newRes);
    assertSize2dFloatEqual(camera.viewPlaneSize(), Size2d(initViewPlaneSize.width, initViewPlaneSize.height * 2));
    assertSize2dFloatEqual(camera.pixelSize(), initPixelSize);
}

TEST(CameraTest, setResolution_dontAdjustAspectRatio_shouldNotModifyViewPlaneSize) {
    TestCamera camera;

    Camera::Resolution initRes = camera.resolution();
    Size2d initPixelSize = camera.pixelSize();
    Size2d initViewPlaneSize = camera.viewPlaneSize();
    Camera::Resolution newRes{initRes.width, initRes.height * 2};
    camera.setResolution(newRes, false);
    camera.update();

    EXPECT_EQ(camera.resolution(), newRes);
    assertSize2dFloatEqual(camera.viewPlaneSize(), initViewPlaneSize);
    assertSize2dFloatEqual(camera.pixelSize(), Size2d(initPixelSize.width, initPixelSize.height / 2));
}

TEST(CameraTest, setAspectRatio_shouldAdjustViewPlaneHeight) {
    TestCamera camera;

    Size2d initViewPlaneSize = camera.viewPlaneSize();
    float newAspectRatio = 2.0f;
    camera.setAspectRatio(newAspectRatio);
    camera.update();

    EXPECT_FLOAT_EQ(camera.aspectRatio(), newAspectRatio);
    assertSize2dFloatEqual(camera.viewPlaneSize(), Size2d(initViewPlaneSize.width, initViewPlaneSize.width / 2));
}

TEST(CameraTest, setViewPlaneWidth_shouldKeepAspectRatio) {
    TestCamera camera;

    Size2d initPixelSize = camera.pixelSize();
    float initAspectRatio = camera.aspectRatio();
    Size2d initViewPlaneSize = camera.viewPlaneSize();
    float increaseFactor = 1.5f;
    float newViewPlaneWidth = initViewPlaneSize.width * increaseFactor;
    camera.setViewPlaneWidth(newViewPlaneWidth);
    camera.update();

    EXPECT_FLOAT_EQ(camera.aspectRatio(), initAspectRatio);
    assertSize2dFloatEqual(camera.viewPlaneSize(),
                           Size2d(newViewPlaneWidth, initViewPlaneSize.height * increaseFactor));
    assertSize2dFloatEqual(camera.pixelSize(),
                           Size2d(initPixelSize.width * increaseFactor, initPixelSize.height * increaseFactor));
}

TEST(CameraTest, setViewPlaneSize_shouldAffectPixelSize) {
    TestCamera camera;

    Size2d initPixelSize = camera.pixelSize();
    float initAspectRatio = camera.aspectRatio();
    Size2d initViewPlaneSize = camera.viewPlaneSize();
    float widthIncreaseFactor = 1.5f;
    float heightIncreaseFactor = 2.0f;
    Size2d newViewPlaneSize = {initViewPlaneSize.width * widthIncreaseFactor,
                               initViewPlaneSize.height * heightIncreaseFactor};
    camera.setViewPlaneSize(newViewPlaneSize);
    camera.update();

    assertSize2dFloatEqual(camera.viewPlaneSize(), newViewPlaneSize);
    assertSize2dFloatEqual(camera.pixelSize(), Size2d(initPixelSize.width * widthIncreaseFactor,
                                                      initPixelSize.height * heightIncreaseFactor));
    EXPECT_FLOAT_EQ(camera.aspectRatio(), initAspectRatio * widthIncreaseFactor / heightIncreaseFactor);
}

TEST(CameraTest, update_shouldCallOnUpdated) {
    TestCamera camera;

    camera.update();
    EXPECT_EQ(camera.onUpdatedCallCount, 1);
}

TEST(CameraTest, cameraTransform_shouldReturnExpected) {
    TestCamera camera;

    camera.setPosition(1, 0, 1);
    camera.setViewDirection(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
    camera.update();

    glm::mat4 expected = {{0, 0, -1, 0}, {0, 1, 0, 0}, {1, 0, 0, 0}, {-1, 0, 1, 1}};
    glm::mat4 cameraTransform = camera.cameraTransform();
    assertMat4FloatEqual(cameraTransform, expected);
    assertVec4FloatEqual(cameraTransform * glm::vec4(4, 0, 2, 1), glm::vec4(1, 0, -3, 1));
}

TEST(CameraTest, viewportTransform_shouldReturnExpected) {
    TestCamera camera;

    camera.setResolution({160, 100});
    camera.update();

    glm::mat4 expected = {{80, 0, 0, 0}, {0, 50, 0, 0}, {0, 0, 1, 0}, {79.5f, 49.5, 0, 1}};
    glm::mat4 viewportTransform = camera.viewportTransform();
    assertMat4FloatEqual(viewportTransform, expected);
    assertVec4FloatEqual(viewportTransform * glm::vec4(0.5f, 0, 0.34f, 1), glm::vec4(119.5, 49.5, 0.34f, 1));
}

#ifndef NDEBUG
TEST(CameraTest, setViewPlaneDistance_zeroDistance_shouldTriggerAssert) {
    TestCamera camera;

    ASSERT_DEATH(camera.setViewPlaneDistance(0), ".*");
}

TEST(CameraTest, setViewPlaneDistance_distanceBeyondLimit_shouldTriggerAssert) {
    TestCamera camera;

    ASSERT_DEATH(camera.setViewPlaneDistance(camera.viewLimit() + 1), ".*");
}

TEST(CameraTest, setViewPlaneLimit_limitBeforeDistance_shouldTriggerAssert) {
    TestCamera camera;

    ASSERT_DEATH(camera.setViewLimit(camera.viewPlaneDistance() / 2), ".*");
}
#endif
