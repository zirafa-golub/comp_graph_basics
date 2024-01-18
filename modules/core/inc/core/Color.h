#pragma once

#include <type_traits>

#include "glm/vec3.hpp"

namespace cg {
class Color {
public:
    Color(float r, float g, float b) : rgb_({r, g, b}) {}
    Color(const glm::vec3& rgb) : rgb_(rgb) {}

    float r() const { return rgb_[0]; }
    float g() const { return rgb_[1]; }
    float b() const { return rgb_[2]; }

    glm::vec3& data() { return rgb_; }
    const glm::vec3& data() const { return rgb_; }

    bool operator==(const Color& other) const { return rgb_ == other.rgb_; }
    bool operator!=(const Color& other) const { return !(*this == other); }

    Color operator+(const Color& other) { return rgb_ + other.rgb_; }
    Color operator-(const Color& other) { return rgb_ - other.rgb_; }
    Color operator*(const Color& other) { return rgb_ * other.rgb_; }
    Color operator*(float scalar) const { return scalar * rgb_; }
    friend Color operator*(float scalar, const Color& color) { return color * scalar; }
    Color operator/(float scalar) const { return rgb_ / scalar; }

private:
    glm::vec3 rgb_;
};
} // namespace cg