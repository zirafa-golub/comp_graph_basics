#include "gtest/gtest.h"

#include "core/Camera.h"

using namespace cg;

struct TestCamera : public Camera {
    unsigned updateCalls = 0;

    Ray castRay(unsigned pixelX, unsigned pixelY) const override { return Ray({0, 0, 0}, {0, 0, 1}); }
    const glm::vec3& rightVector() { return Camera::rightVector(); }
    void update() override {
        Camera::update();
        ++updateCalls;
    }
};

TEST(CameraTest, modify_shouldApplyModificationAfterScope) {
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
    EXPECT_EQ(camera.updateCalls, 1);
}