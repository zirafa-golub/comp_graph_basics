#pragma once

#include "renderer/Screen.h"

#include <span>
#include <vector>

namespace cg {
class MemoryColorBuffer {
public:
    class Painter {
    public:
        Painter(int width, int height, std::span<Color> buffer) : width_(width), height_(height), buffer_(buffer) {}

        void paint(int row, int col, const Color& color) { buffer_[row * width_ + col] = color; }
        int width() { return width_; }
        int height() { return height_; }

    private:
        int width_;
        int height_;
        std::span<Color> buffer_;
    };

    static_assert(PixelPainter<Painter>, "MemoryColorBuffer::Painter does not fulfill the PixelPainter concept.");

    MemoryColorBuffer(int width, int height);

    int width();
    int height();
    void clear(Color color = Color::black());
    Painter paintPixels();
    Color colorAtPixel(int row, int col) { return buffer_[row * width_ + col]; }
    void flush() {}

private:
    int width_;
    int height_;
    std::vector<Color> buffer_;
};

static_assert(Screen<MemoryColorBuffer>, "MemoryColorBuffer does not fulfill the Screen concept.");
} // namespace cg
