#pragma once

#include "core/BasicTypes.h"
#include "core/LineEquation2d.h"

#include "glm/geometric.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <functional>

namespace cg {
struct FragmentData {
    int x;
    int y;
    float z;
    glm::vec3 normal;
    Point pos3d;
};

template <typename T>
concept FragmentPainter = requires(T painter, const FragmentData& fragmentData) {
    { painter.paintFragment(fragmentData) } -> std::same_as<void>;
    { painter.width() } -> std::convertible_to<int>;
    { painter.height() } -> std::convertible_to<int>;
};

class TriangleRasterizer {
public:
    TriangleRasterizer() = delete;

    template <FragmentPainter Painter>
    static void rasterize(std::array<std::reference_wrapper<const Point>, 3> homogenizedScreenPoints,
                          std::array<std::reference_wrapper<const glm::vec3>, 3> pos3ds,
                          std::array<std::reference_wrapper<const glm::vec3>, 3> normals,
                          const std::array<float, 3>& invertedW, Painter& fragmentPainter) {
        const Point& p1 = homogenizedScreenPoints[0];
        const Point& p2 = homogenizedScreenPoints[1];
        const Point& p3 = homogenizedScreenPoints[2];
        glm::vec3 dividedN1 = normals[0].get() * invertedW[0];
        glm::vec3 dividedN2 = normals[1].get() * invertedW[1];
        glm::vec3 dividedN3 = normals[2].get() * invertedW[2];

        int minX = std::max(static_cast<int>(std::min({p1.x, p2.x, p3.x})), 0);
        int maxX = std::min(static_cast<int>(std::max({p1.x, p2.x, p3.x})) + 1, fragmentPainter.width());
        int minY = std::max(static_cast<int>(std::min({p1.y, p2.y, p3.y})), 0);
        int maxY = std::min(static_cast<int>(std::max({p1.y, p2.y, p3.y})) + 1, fragmentPainter.height());

        LineEquation2d line12(p1, p2);
        LineEquation2d line23(p2, p3);
        LineEquation2d line31(p3, p1);

        float fAlpha = line23.eval(p1);
        float fBeta = line31.eval(p2);
        float fGamma = line12.eval(p3);

        // TODO: test incremental algorithm
        for (int y = minY; y < maxY; ++y) {
            for (int x = minX; x < maxX; ++x) {
                float fpx = static_cast<float>(x);
                float fpy = static_cast<float>(y);

                float alpha = line23.eval(fpx, fpy) / fAlpha;
                // Checks for alpha, beta and gamma below are intentionally formed like this instead of alpha < 0 so NaN
                // values would fail the check as well
                if (!(alpha >= 0)) {
                    continue;
                }
                float beta = line31.eval(fpx, fpy) / fBeta;
                if (!(beta >= 0)) {
                    continue;
                }
                float gamma = line12.eval(fpx, fpy) / fGamma;
                if (!(gamma >= 0)) {
                    continue;
                }

                if ((alpha > 0 || shouldDrawWhenOnEdge(line23)) && (beta > 0 || shouldDrawWhenOnEdge(line31)) &&
                    (gamma > 0 || shouldDrawWhenOnEdge(line12))) {
                    FragmentData fragmentData;
                    fragmentData.x = x;
                    fragmentData.y = y;
                    float fragInvW = alpha * invertedW[0] + beta * invertedW[1] + gamma * invertedW[2];
                    fragmentData.pos3d =
                        (alpha * invertedW[0] * pos3ds[0].get() + beta * invertedW[1] * pos3ds[1].get() +
                         gamma * invertedW[2] * pos3ds[2].get()) /
                        fragInvW;
                    fragmentData.z = alpha * p1.z + beta * p2.z + gamma * p3.z;
                    fragmentData.normal =
                        glm::normalize((alpha * dividedN1 + beta * dividedN2 + gamma * dividedN3) / fragInvW);

                    fragmentPainter.paintFragment(fragmentData);
                }
            }
        }
    }

private:
    static bool shouldDrawWhenOnEdge(const LineEquation2d& edgeLine) {
        // Draw if edge is either a "left" (a > 0) or "top" (a == 0 && b < 0) edge of the triangle
        return (edgeLine.a() > 0) || (edgeLine.a() == 0 && edgeLine.b() < 0);
    }
};
} // namespace cg
