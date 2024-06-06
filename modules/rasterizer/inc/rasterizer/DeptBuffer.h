#pragma once

#include <vector>

namespace cg {
class DepthBuffer {
public:
    static constexpr float farthest = -1.0f;

    DepthBuffer(int width, int height);

    bool updateIfNearer(int x, int y, float newDepth);
    void clear();

private:
    int width_;
    int height_;
    std::vector<float> buffer_;
};
} // namespace cg
