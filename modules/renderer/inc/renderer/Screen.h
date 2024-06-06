#pragma once

#include "core/Color.h"

#include <concepts>

namespace cg {

template <typename T>
concept PixelPainter = requires(T pp, const Color& color) {
    { pp.paintPixel(int(), int(), color) } -> std::same_as<void>;
    { pp.width() } -> std::convertible_to<int>;
    { pp.height() } -> std::convertible_to<int>;
};

template <typename T>
concept Screen = requires(T screen, const Color& color) {
    { screen.width() } -> std::convertible_to<int>;
    { screen.height() } -> std::convertible_to<int>;
    { screen.clearScreen(color) } -> std::same_as<void>;
    { screen.paintPixels() } -> PixelPainter;
    { screen.flush() } -> std::same_as<void>;
};
} // namespace cg
