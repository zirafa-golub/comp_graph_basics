#include "rasterizer/DepthBuffer.h"

#include "gtest/gtest.h"

using namespace cg;

TEST(DepthBufferTest, updateIfNearer_bothCases_shouldReturnExpected) {
    DepthBuffer buff(10, 10);

    bool isUpdated = buff.updateIfNearer(5, 5, 0.1f);
    EXPECT_TRUE(isUpdated);

    isUpdated = buff.updateIfNearer(5, 5, -0.1f);
    EXPECT_FALSE(isUpdated);

    buff.clear();
    isUpdated = buff.updateIfNearer(5, 5, -0.1f);
    EXPECT_TRUE(isUpdated);
}

TEST(DepthBufferTest, depthAtPixel_shouldReturnExpected) {
    constexpr float depth = 0.1f;
    DepthBuffer buff(10, 10);

    bool isUpdated = buff.updateIfNearer(5, 5, depth);
    EXPECT_TRUE(isUpdated);

    EXPECT_EQ(buff.depthAtPixel(5, 5), depth);
}
