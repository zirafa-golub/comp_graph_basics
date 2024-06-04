#pragma once

#include "core/BasicTypes.h"
#include "core/Color.h"
#include "core/LineEquation2d.h"
#include "renderer/Screen.h"

#include <algorithm>
#include <cmath>

namespace cg {
class TriangleRasterizer {
public:
    TriangleRasterizer() = delete;

    template <PixelPainter Painter>
    static void rasterize(const Point& p1, const Point& p2, const Point& p3, Painter& painter) {
        int minX = std::max(static_cast<int>(std::min({p1.x, p2.x, p3.x})), 0);
        int maxX = std::min(static_cast<int>(std::max({p1.x, p2.x, p3.x})) + 1, painter.width());
        int minY = std::max(static_cast<int>(std::min({p1.y, p2.y, p3.y})), 0);
        int maxY = std::min(static_cast<int>(std::max({p1.y, p2.y, p3.y})) + 1, painter.height());

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
                // Checks for alpha, beta and gamma below are intentionally formed like this instead of alpha > 0 so NaN
                // values would fail the check as well
                if (!(alpha >= 0)) {
                    continue;
                }
                float beta = line31.eval(fpx, fpy) / fBeta;
                if (!(beta >= 0)) {
                    continue;
                }
                float gamma = 1 - alpha - beta;
                if (!(gamma >= 0)) {
                    continue;
                }

                if ((alpha > 0 || shouldDrawWhenOnEdge(line23)) && (beta > 0 || shouldDrawWhenOnEdge(line31)) &&
                    (gamma > 0 || shouldDrawWhenOnEdge(line12))) {
                    painter.paintPixel(y, x, Color::white());
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
