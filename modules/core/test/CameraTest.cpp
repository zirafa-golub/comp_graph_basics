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
    Camera::Resolution newRes{2 * initRes.width, 3 * initRes.height};
    glm::vec3 newDir = -camera.viewDirection();
    glm::vec3 newUp = camera.upVector();
    Size2d initViewPlane = camera.viewPlaneSize();
    Size2d newViewPlane{initViewPlane.width / 2, initViewPlane.height / 3};

    {
        auto modifier = camera.modify();
        modifier.setResolution(newRes);
        EXPECT_EQ(camera.resolution(), newRes);
        modifier.setViewDirection(newDir, {1, 1, 0});
        EXPECT_EQ(camera.viewDirection(), newDir);
        modifier.setViewPlaneSize(newViewPlane);
        EXPECT_EQ(camera.viewPlaneSize(), newViewPlane);

        EXPECT_EQ(initRight, camera.rightVector());
        EXPECT_EQ(initUp, camera.upVector());
        EXPECT_EQ(initPixelSize, camera.pixelSize());
    }

    EXPECT_EQ(camera.rightVector(), glm::vec3({0, 0, -1}));
    EXPECT_EQ(camera.upVector(), glm::vec3({0, 1, 0}));
    EXPECT_EQ(camera.pixelSize(), Size2d(initPixelSize.width / 4, initPixelSize.height / 9));
    EXPECT_EQ(camera.updateCalls, 1);
}