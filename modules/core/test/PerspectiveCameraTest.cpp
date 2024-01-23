#include "gtest/gtest.h"

#include "core/PerspectiveCamera.h"

using namespace cg;
using namespace cg::angle_literals;

TEST(PerspectiveCameraTest, setFieldOfView_shouldAffectViewPlaneAfterUpdate) {
    PerspectiveCamera camera;

    Size2d initViewPlaneSize = camera.viewPlaneSize();
    float initDistance = camera.viewPlaneDistance();

    Angle fov = Angle::deg(90);
    EXPECT_NE(fov, camera.fieldOfView());

    camera.setFieldOfView(fov);

    EXPECT_FLOAT_EQ(fov.asRad(), camera.fieldOfView().asRad());
    EXPECT_FLOAT_EQ(initDistance * 2, camera.viewPlaneSize().width);
}

TEST(PerspectiveCameraTest, castRay_evenPixelCount_shouldReturnExpectedRay) {
    PerspectiveCamera camera;

    camera.setPosition(1, 2, 3);
    camera.setResolution({100, 100});
    camera.setViewDirection({2, 0, 0}, {0, 3, 0});
    camera.setViewPlaneDistance(50);
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
    camera.setFieldOfView(Angle::deg(90));
    camera.update();

    Ray ray = camera.castRay(52, 63);

    EXPECT_EQ(ray.origin(), Point(104, 32, 8));
    EXPECT_EQ(ray.direction(), glm::vec3(99, 28, 6));
}

#ifndef NDEBUG
TEST(PerspectiveCameraTest, setViewPlaneDistance_invalidDistance_shouldTriggerAssert) {
    PerspectiveCamera camera;

    ASSERT_DEATH(camera.setViewPlaneDistance(0), ".*");
}

TEST(PerspectiveCameraTest, castRay_invalidPixel_shouldTriggerAssert) {
    PerspectiveCamera camera;

    camera.setResolution({100, 100});
    camera.update();

    ASSERT_DEATH(camera.castRay(100, 100), ".*");
}
#endif
