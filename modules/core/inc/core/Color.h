#pragma once

#include <cstdint>
#include <type_traits>

#include "glm/ext/vector_common.hpp"
#include "glm/vec3.hpp"

namespace cg {
class Color {
public:
    constexpr Color() : rgb_(0, 0, 0) {}
    template <typename R, typename G, typename B>
    constexpr Color(R r, G g, B b) : rgb_({r, g, b}) {}
    constexpr explicit Color(const glm::vec3& rgb) : rgb_(rgb) {}

    constexpr float r() const { return rgb_[0]; }
    constexpr float g() const { return rgb_[1]; }
    constexpr float b() const { return rgb_[2]; }

    constexpr glm::vec3& data() { return rgb_; }
    constexpr const glm::vec3& data() const { return rgb_; }

    constexpr Color clamp() const { return Color(glm::clamp(rgb_, 0.0f, 1.0f)); }

    constexpr bool operator==(const Color& other) const = default;

    constexpr Color operator+(const Color& other) const { return Color(rgb_ + other.rgb_); }
    constexpr Color& operator+=(const Color& other) {
        rgb_ += other.rgb_;
        return *this;
    }
    constexpr Color operator-(const Color& other) const { return Color(rgb_ - other.rgb_); }
    constexpr Color& operator-=(const Color& other) {
        rgb_ -= other.rgb_;
        return *this;
    }
    constexpr Color operator*(const Color& other) const { return Color(rgb_ * other.rgb_); }
    constexpr Color operator*(float scalar) const { return Color(scalar * rgb_); }
    constexpr friend Color operator*(float scalar, const Color& color) { return color * scalar; }
    constexpr Color operator/(float scalar) const { return Color(rgb_ / scalar); }

    static constexpr Color black() { return Color(0, 0, 0); }
    static constexpr Color white() { return Color(1, 1, 1); }
    static constexpr Color red() { return Color(1, 0, 0); }
    static constexpr Color green() { return Color(0, 1, 0); }
    static constexpr Color blue() { return Color(0, 0, 1); }

private:
    glm::vec3 rgb_;
};
} // namespace cg
