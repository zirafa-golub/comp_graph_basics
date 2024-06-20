#include "core/Sphere.h"
#include "test_utils/Utils.h"

#include "glm/geometric.hpp"
#include "gtest/gtest.h"

#include <iomanip>
#include <limits>

using namespace cg;

TEST(SphereTest, center_shouldReturnPosition) {
    Sphere sphere{3};
    Point p{1, 2, 3};
    sphere.setPosition(p);

    EXPECT_EQ(sphere.center(), p);
}
