#pragma once

#include "core/Color.h"

namespace cg {

template <typename T>
concept PixelPainter = requires(T pp, const Color& color) {
    { pp.paintPixel(unsigned(), unsigned(), color) } -> std::same_as<void>;
    { pp.width() } -> std::convertible_to<unsigned>;
    { pp.height() } -> std::convertible_to<unsigned>;
};

template <typename T>
concept Screen = requires(T screen, const Color& color) {
    { screen.width() } -> std::convertible_to<unsigned>;
    { screen.height() } -> std::convertible_to<unsigned>;
    { screen.clearScreen(color) } -> std::same_as<void>;
    { screen.paintPixels() } -> PixelPainter;
    { screen.flush() } -> std::same_as<void>;
};
} // namespace cg
