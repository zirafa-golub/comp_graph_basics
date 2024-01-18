#include "gtest/gtest.h"

#include "core/Color.h"

using namespace cg;

void assertColorsFloatEqual(const Color& left, const Color& right) {
    EXPECT_FLOAT_EQ(left.r(), right.r());
    EXPECT_FLOAT_EQ(left.g(), right.g());
    EXPECT_FLOAT_EQ(left.b(), right.b());
}