#include "rasterizer/DepthBuffer.h"

#include <algorithm>
#include <cassert>

namespace cg {
DepthBuffer::DepthBuffer(int width, int height) : width_(width), height_(height), buffer_(width_ * height_, farthest) {
    assert(width_ > 0);
    assert(height_ > 0);
}

bool DepthBuffer::updateIfNearer(int x, int y, float newDepth) {
    assert(newDepth > -1.0f && newDepth < 1.0f);
    float& depthPixel = buffer_[y * width_ + x];
    if (newDepth > depthPixel) {
        depthPixel = newDepth;
        return true;
    }
    return false;
}

void DepthBuffer::clear() { std::fill(buffer_.begin(), buffer_.end(), farthest); }

} // namespace cg
