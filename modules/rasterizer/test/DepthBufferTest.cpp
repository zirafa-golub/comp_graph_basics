#include "rasterizer/DeptBuffer.h"

#include "gtest/gtest.h"

using namespace cg;

TEST(DepthBufferTest, updateIfNearer_bothCases_shouldReturnExpected) {
    DepthBuffer buff(10, 10);

    bool isUpdated = buff.updateIfNearer(5, 5, -0.1f);
    EXPECT_TRUE(isUpdated);

    isUpdated = buff.updateIfNearer(5, 5, 0.1f);
    EXPECT_FALSE(isUpdated);

    buff.clear();
    isUpdated = buff.updateIfNearer(5, 5, 0.1f);
    EXPECT_TRUE(isUpdated);
}
