#pragma once

#include <span>
#include <vector>

#include <new>

namespace cg {
class DepthBuffer {
public:
    static constexpr float farthest = -1.0f;

    DepthBuffer(int width, int height);

    bool updateIfNearer(int x, int y, float newDepth);
    int width() { return width_; }
    int height() { return height_; }
    void clear();
    float depthAtPixel(int x, int y) { return buffer_[y * width_ + x]; }

private:
    int width_;
    int height_;
    std::vector<float> buffer_;
};
} // namespace cg
