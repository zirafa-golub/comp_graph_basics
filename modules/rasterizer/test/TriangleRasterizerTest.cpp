#include "core/Color.h"
#include "rasterizer/TriangleRasterizer.h"

#include "gtest/gtest.h"

#include <set>

using namespace cg;

class TestPainter {
public:
    TestPainter(int width, int height) : width_(width), height_(height), pixels_(width_ * height_) {}

    void paintPixel(unsigned row, unsigned col, const Color& color) {
        pixels_[row * width_ + col] = color;
        ++pixelsDrawn_;
    }
    int width() const { return width_; }
    int height() const { return height_; }

    const Color& getPixel(unsigned row, unsigned col) const { return pixels_[row * width_ + col]; }
    void clear() {
        pixels_.clear();
        pixels_.resize(width_ * height_, Color::black());
        pixelsDrawn_ = 0;
    }
    unsigned pixelsDrawn() const { return pixelsDrawn_; }
    const std::vector<Color>& pixels() { return pixels_; }

private:
    int width_;
    int height_;
    std::vector<Color> pixels_;
    unsigned pixelsDrawn_ = 0;
};

struct Vector2i {
    int x;
    int y;

    friend auto operator<=>(const Vector2i&, const Vector2i&) = default;
    friend bool operator<(const Vector2i&, const Vector2i&) = default;
};

void assertColoredPixels(std::set<Vector2i> coloredPixels, const TestPainter& painter) {
    EXPECT_EQ(coloredPixels.size(), painter.pixelsDrawn());
    for (int x = 0; x < painter.width(); ++x) {
        for (int y = 0; y < painter.height(); ++y) {
            if (coloredPixels.contains(Vector2i(x, y))) {
                EXPECT_EQ(painter.getPixel(y, x), Color::white()) << "x: " << x << ", y: " << y;
            } else {
                EXPECT_EQ(painter.getPixel(y, x), Color::black()) << "x: " << x << ", y: " << y;
            }
        }
    }
}

TEST(TriangleRasterizerTest, rasterize_triangleOffScreen_shouldDrawNothing) {
    constexpr int width = 10;
    constexpr int height = 8;

    TestPainter painter(width, height);

    // triangle to the left of screen
    TriangleRasterizer::rasterize({-5, 3, 0}, {-2, 2, 0}, {-3, 6, 0}, painter);
    assertColoredPixels({}, painter);
    painter.clear();
    // triangle to the right of screen
    TriangleRasterizer::rasterize({12, 3, 0}, {16, 2, 0}, {13, 6, 0}, painter);
    assertColoredPixels({}, painter);
    painter.clear();
    // triangle above screen
    TriangleRasterizer::rasterize({4, 10, 0}, {7, 11, 0}, {5, 14, 0}, painter);
    assertColoredPixels({}, painter);
    painter.clear();
    // triangle below screen
    TriangleRasterizer::rasterize({4, -1, 0}, {7, -2, 0}, {5, -4, 0}, painter);
    assertColoredPixels({}, painter);
    painter.clear();
}

TEST(TriangleRasterizerTest, rasterize_trianglePartiallyOnScreen_shouldDrawOnlyVisiblePart) {
    constexpr int width = 10;
    constexpr int height = 8;

    TestPainter painter(width, height);

    TriangleRasterizer::rasterize({0.8f, 5.8f, 0}, {5.2f, 9.2f, 0}, {0.8f, 9.2f, 0}, painter);
    assertColoredPixels({{1, 7}, {2, 7}, {1, 6}}, painter);
}

TEST(TriangleRasterizerTest, rasterize_triangleOnScreen_shouldDrawTriangle) {
    constexpr int width = 10;
    constexpr int height = 8;

    TestPainter painter(width, height);

    TriangleRasterizer::rasterize({2, 2.7f, 0}, {3.2, 5.2, 0}, {1.8f, 5.2f, 0}, painter);
    assertColoredPixels({{2, 5}, {3, 5}, {2, 4}, {2, 3}}, painter);
}

TEST(TriangleRasterizerTest, rasterize_sameTriangleDifferentZ_shouldDrawSame) {
    constexpr int width = 10;
    constexpr int height = 8;

    TestPainter painter(width, height);

    TriangleRasterizer::rasterize({2, 2.7f, 0}, {3.2, 5.2, 0}, {1.8f, 5.2f, 0}, painter);
    auto first = painter.pixels();

    painter.clear();

    TriangleRasterizer::rasterize({2, 2.7f, 1}, {3.2, 5.2, -2}, {1.8f, 5.2f, 3}, painter);
    auto second = painter.pixels();

    EXPECT_EQ(first, second);
}

TEST(TriangleRasterizerTest, rasterize_twoTrianglesPixelsOnSharedEdge_shouldDrawEveryPixelOnce) {
    constexpr int width = 10;
    constexpr int height = 8;

    TestPainter painter(width, height);

    TriangleRasterizer::rasterize({5, 5.5f, 0}, {5, 2.5f, 0}, {6.5f, 4, 0}, painter);
    TriangleRasterizer::rasterize({5, 5.5f, 0}, {3.5f, 4, 0}, {5, 2.5f, 0}, painter);

    std::set<Vector2i> coloredPixels = {{5, 5}, {4, 4}, {5, 4}, {6, 4}, {5, 3}};
    assertColoredPixels(coloredPixels, painter);
}

TEST(TriangleRasterizerTest, rasterize_degenerateTriangle_shouldDrawNothing) {
    constexpr unsigned width = 10;
    constexpr unsigned height = 8;

    TestPainter painter(width, height);

    // All pixels on edge
    TriangleRasterizer::rasterize({5, 5, 0}, {5, 4, 0}, {5, 2, 0}, painter);
    assertColoredPixels({}, painter);
    painter.clear();

    // Pixels not on edge
    TriangleRasterizer::rasterize({4.8f, 5, 0}, {5, 4, 0}, {5.2f, 3, 0}, painter);
    assertColoredPixels({}, painter);
}
