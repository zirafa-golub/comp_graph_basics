#pragma once

#include <compare>
#include <numbers>
#include <type_traits>

namespace cg {

class Angle {
public:
    Angle() : angleRad_(0.0f) {}
    Angle(const Angle&) = default;
    Angle(Angle&&) = default;

    constexpr Angle& operator=(const Angle&) = default;
    constexpr Angle& operator=(Angle&&) = default;

    static constexpr Angle rad(float radians) { return Angle(radians); }
    static constexpr Angle deg(float degrees) { return Angle(degrees * radToDegRatio); }

    constexpr float asDeg() const { return angleRad_ / radToDegRatio; }
    constexpr float asRad() const { return angleRad_; }

    constexpr auto operator<=>(const Angle&) const = default;

    constexpr Angle operator+(const Angle& other) { return Angle{angleRad_ + other.angleRad_}; }
    constexpr Angle& operator+=(const Angle& other) {
        angleRad_ += other.angleRad_;
        return *this;
    }
    constexpr Angle operator-(const Angle& other) { return Angle{angleRad_ - other.angleRad_}; }
    constexpr Angle& operator-=(const Angle& other) {
        angleRad_ -= other.angleRad_;
        return *this;
    }

    template <typename T>
    constexpr friend Angle operator*(const Angle& angle, T scalar)
        requires std::is_arithmetic_v<T>
    {
        return Angle(angle.angleRad_ * scalar);
    }

    template <typename T>
    constexpr friend Angle operator*(T scalar, const Angle& angle)
        requires std::is_arithmetic_v<T>
    {
        return Angle(angle.angleRad_ * static_cast<float>(scalar));
    }

    template <typename T>
    constexpr Angle& operator*=(T scalar)
        requires std::is_arithmetic_v<T>
    {
        angleRad_ *= static_cast<float>(scalar);
        return *this;
    }

    template <typename T>
    constexpr Angle operator/(T scalar)
        requires std::is_arithmetic_v<T>
    {
        return Angle(angleRad_ / static_cast<float>(scalar));
    }

    template <typename T>
    constexpr Angle& operator/=(T scalar)
        requires std::is_arithmetic_v<T>
    {
        angleRad_ /= static_cast<float>(scalar);
        return *this;
    }

private:
    static constexpr float radToDegRatio = std::numbers::pi_v<float> / 180;

    explicit constexpr Angle(float angleRad) : angleRad_(angleRad) {}

    float angleRad_;
};

namespace angle_literals {
constexpr Angle operator""_rad(long double radians) { return Angle::rad(static_cast<float>(radians)); }
constexpr Angle operator""_rad(unsigned long long int radians) { return Angle::rad(static_cast<float>(radians)); }
constexpr Angle operator""_deg(long double degrees) { return Angle::deg(static_cast<float>(degrees)); }
constexpr Angle operator""_deg(unsigned long long int degrees) { return Angle::deg(static_cast<float>(degrees)); }
} // namespace angle_literals

} // namespace cg
