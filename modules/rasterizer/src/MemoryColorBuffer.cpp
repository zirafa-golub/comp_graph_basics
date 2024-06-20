#include "rasterizer/MemoryColorBuffer.h"

#include <algorithm>

namespace cg {
MemoryColorBuffer::MemoryColorBuffer(int width, int height)
    : width_(width), height_(height), buffer_(width_ * height, Color::black()) {}

int MemoryColorBuffer::width() { return width_; }
int MemoryColorBuffer::height() { return height_; }
void MemoryColorBuffer::clear(Color color) { std::fill(buffer_.begin(), buffer_.end(), color); }
MemoryColorBuffer::Painter MemoryColorBuffer::paintPixels() {
    return MemoryColorBuffer::Painter(width_, height_, buffer_);
}
} // namespace cg
