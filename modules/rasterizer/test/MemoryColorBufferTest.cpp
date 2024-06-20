#include "rasterizer/MemoryColorBuffer.h"

#include "gtest/gtest.h"

using namespace cg;

TEST(MemoryColorBufferTest, constructor_shouldClearToBlack) {
    constexpr int width = 3;
    constexpr int height = 4;
    MemoryColorBuffer buffer(width, height);
    EXPECT_EQ(buffer.width(), width);
    EXPECT_EQ(buffer.height(), height);
    for (int y = 0; y < buffer.height(); ++y) {
        for (int x = 0; x < buffer.width(); ++x) {
            EXPECT_EQ(buffer.colorAtPixel(x, y), Color::black()) << "x: " << x << ", y:" << y;
        }
    }
}

TEST(MemoryColorBufferTest, clear_shouldClearToSpecifiedColor) {
    constexpr int width = 3;
    constexpr int height = 4;
    MemoryColorBuffer buffer(width, height);
    buffer.clear(Color::red());
    for (int y = 0; y < buffer.height(); ++y) {
        for (int x = 0; x < buffer.width(); ++x) {
            EXPECT_EQ(buffer.colorAtPixel(x, y), Color::red()) << "x: " << x << ", y:" << y;
        }
    }
}

TEST(MemoryColorBufferTest, painter_shouldPaintExpectedPixels) {
    constexpr int width = 3;
    constexpr int height = 4;
    constexpr int targetRow = 1;
    constexpr int targetCol = 2;

    MemoryColorBuffer buffer(width, height);
    EXPECT_EQ(buffer.colorAtPixel(targetRow, targetCol), Color::black());
    auto painter = buffer.paintPixels();
    painter.paint(targetRow, targetCol, Color::blue());
    EXPECT_EQ(buffer.colorAtPixel(targetRow, targetCol), Color::blue());
}
