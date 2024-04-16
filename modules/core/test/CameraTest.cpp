#include "gtest/gtest.h"

#include "core/Camera.h"
#include "TestUtils.h"

using namespace cg;

struct TestCamera : public Camera {
    Ray castRay(unsigned pixelX, unsigned pixelY) const override { return Ray({0, 0, 0}, {0, 0, 1}); }
    const glm::vec3& rightVector() { return Camera::rightVector(); }
};

TEST(CameraTest, modify_shouldApplyModificationAfterUpdate) {
    TestCamera camera;

    glm::vec3 initRight = camera.rightVector();
    glm::vec3 initUp = camera.upVector();
    Size2d initPixelSize = camera.pixelSize();

    Camera::Resolution initRes = camera.resolution();
    Camera::Resolution newRes{200, 300};
    glm::vec3 newDir = {-2, 0, 0};
    glm::vec3 newUp = {1, 0, 1};
    Size2d initViewPlane = camera.viewPlaneSize();
    Size2d newViewPlane{initViewPlane.width / 2, initViewPlane.height / 3};

    camera.setResolution(newRes);
    EXPECT_EQ(camera.resolution(), newRes);
    camera.setViewDirection(newDir, newUp);
    EXPECT_EQ(camera.viewDirection(), newDir);
    camera.setViewPlaneSize(newViewPlane);
    EXPECT_EQ(camera.viewPlaneSize(), newViewPlane);

    EXPECT_EQ(initRight, camera.rightVector());
    EXPECT_EQ(initPixelSize, camera.pixelSize());

    camera.update();

    EXPECT_EQ(camera.rightVector(), glm::vec3({0, 1, 0}));
    EXPECT_EQ(camera.upVector(), glm::vec3({0, 0, 1}));
    EXPECT_EQ(camera.pixelSize(), Size2d(initPixelSize.width / 4, initPixelSize.height / 9));
}

TEST(CameraTest, viewVectorModifiers_shouldReturnExpected) {
    TestCamera camera;

    glm::vec3 initRight = camera.rightVector();
    glm::vec3 initUp = camera.upVector();

    glm::vec3 newDir = {-2, 0, 0};
    glm::vec3 newUp = {1, 0, 1};

    camera.setViewDirection(newDir, newUp);
    camera.update();

    EXPECT_EQ(camera.viewDirection(), glm::vec3(-1, 0, 0));
    EXPECT_EQ(camera.rightVector(), glm::vec3(0, 1, 0));
    EXPECT_EQ(camera.upVector(), glm::vec3(0, 0, 1));
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
