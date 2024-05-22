#include "core/Color.h"
#include "core/TestUtils.h"

#include "glm/geometric.hpp"
#include "gtest/gtest.h"

using namespace cg;

void assertColorsFloatEqual(const Color& left, const Color& right) {
    EXPECT_FLOAT_EQ(left.r(), right.r());
    EXPECT_FLOAT_EQ(left.g(), right.g());
    EXPECT_FLOAT_EQ(left.b(), right.b());
}

void assertSize2dFloatEqual(const Size2d& left, const Size2d& right) {
    EXPECT_FLOAT_EQ(left.width, right.width);
    EXPECT_FLOAT_EQ(left.height, right.height);
}
