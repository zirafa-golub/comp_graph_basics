#include "gtest/gtest.h"

#include "core/OrthogonalCamera.h"
#include <iostream>

using namespace cg;

TEST(OrthogonalCameraTest, castRay_evenPixelCount_shouldReturnExpectedRay) {
    OrthogonalCamera camera;

    camera.modify().setResolution({100, 100}).setViewDirection({1, 0, 0}, {0, 1, 0}).setViewPlaneSize({200, 200});

    Ray ray = camera.castRay(10, 12);

    EXPECT_EQ(ray.origin(), Point(0, -75, -79));
    EXPECT_EQ(ray.direction(), glm::vec3(1, 0, 0));
}

TEST(OrthogonalCameraTest, castRay_oddPixelCount_shouldReturnExpectedRay) {
    OrthogonalCamera camera;

    camera.modify().setResolution({99, 99}).setViewDirection({0, 0, 1}, {0, 1, 0}).setViewPlaneSize({99, 99});

    Ray ray = camera.castRay(56, 33);

    std::cout << ray.origin()[0] << " " << ray.origin()[1] << " " << ray.origin()[2] << " " << std::endl;

    EXPECT_EQ(ray.origin(), Point(-7, -16, 0));
    EXPECT_EQ(ray.direction(), glm::vec3(0, 0, 1));
}

#ifndef NDEBUG

TEST(OrthogonalCameraTest, castRay_invalidPixel_shouldTriggerAssert) {
    OrthogonalCamera camera;

    camera.modify().setResolution({100, 100});

    ASSERT_DEATH(camera.castRay(100, 100), ".*");
}

#endif