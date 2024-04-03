#pragma once

#include "core/Angle.h"

#include "glm/vec3.hpp"

namespace cg {
struct Rotation {
    Angle x;
    Angle y;
    Angle z;

    constexpr Rotation() : x(Angle::rad(0)), y(Angle::rad(0)), z(Angle::rad(0)) {}
    constexpr Rotation(Angle x_, Angle y_, Angle z_) : x(x_), y(y_), z(z_) {}
    constexpr Rotation(const Rotation&) = default;
    constexpr Rotation(Rotation&&) = default;

    constexpr Rotation& operator=(const Rotation&) = default;
    constexpr Rotation& operator=(Rotation&&) = default;

    static constexpr Rotation fromVec3Rad(const glm::vec3& rotVec) {
        return Rotation(Angle::rad(rotVec.x), Angle::rad(rotVec.y), Angle::rad(rotVec.z));
    }

    constexpr glm::vec3 toVec3Rad() const { return glm::vec3(x.asRad(), y.asRad(), z.asRad()); }

    constexpr bool operator==(const Rotation& other) const = default;

    constexpr Rotation operator+(const Rotation& other) { return Rotation(x + other.x, y + other.y, z + other.z); }
    constexpr Rotation& operator+=(const Rotation& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    constexpr Rotation operator-(const Rotation& other) { return Rotation(x - other.x, y - other.y, z - other.z); }
    constexpr Rotation& operator-=(const Rotation& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    constexpr Rotation operator-() { return Rotation(-x, -y, -z); }

    template <typename T>
    constexpr friend Rotation operator*(const Rotation& rot, T scalar)
        requires std::is_arithmetic_v<T>
    {
        return Rotation(rot.x * scalar, rot.y * scalar, rot.z * scalar);
    }

    template <typename T>
    constexpr friend Rotation operator*(T scalar, const Rotation& rot)
        requires std::is_arithmetic_v<T>
    {
        return Rotation(rot.x * scalar, rot.y * scalar, rot.z * scalar);
    }

    template <typename T>
    constexpr Rotation& operator*=(T scalar)
        requires std::is_arithmetic_v<T>
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    template <typename T>
    constexpr Rotation operator/(T scalar)
        requires std::is_arithmetic_v<T>
    {
        return Rotation(x / scalar, y / scalar, z / scalar);
    }

    template <typename T>
    constexpr Rotation& operator/=(T scalar)
        requires std::is_arithmetic_v<T>
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
};
} // namespace cg
