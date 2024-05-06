#pragma once

#include "core/BasicTypes.h"

namespace cg {
class LineEquation2d {
public:
    LineEquation2d(float a, float b, float c) : a_(a), b_(b), c_(c) {}
    LineEquation2d(float p1x, float p1y, float p2x, float p2y)
        : LineEquation2d(p1y - p2y, p2x - p1x, p1x * p2y - p2x * p1y) {}
    LineEquation2d(Point2d p1, Point2d p2) : LineEquation2d(p1.x, p1.y, p2.x, p2.y) {}

    float a() const { return a_; }
    float b() const { return b_; }
    float c() const { return c_; }
    float eval(float x, float y) const { return a_ * x + b_ * y + c_; }
    float eval(Point2d p) const { return eval(p.x, p.y); }

private:
    float a_;
    float b_;
    float c_;
};
} // namespace cg
