#include "gtest/gtest.h"

#include "core/Ray.h"

using namespace cg;

TEST(RayTest, eval_shouldReturnExpected) {
    Ray test_ray{{2, 3, 4}, {1, 2, 3}};

    Point p = test_ray.evaluate(2);

    EXPECT_EQ(p, glm::vec3(4, 7, 10));
}