#pragma once

#include "core/Scene.h"
#include "renderer/Screen.h"

#include <concepts>

namespace cg {
namespace detail {
struct ScreenArchetype {
    struct PainterArchetype {
        unsigned width();
        unsigned height();
        void paintPixel(unsigned, unsigned, const Color&);
    };
    static_assert(PixelPainter<PainterArchetype>,
                  "ScreenArchetype::Painter does not fulfill the PixelPainter concept.");

    unsigned width();
    unsigned height();
    void clearScreen(const Color&);
    PainterArchetype paintPixels();
    void flush();
};
static_assert(Screen<ScreenArchetype>, "ScreenArchetype does not fulfill the Screen concept.");
} // namespace detail

template <typename T>
concept Renderer = requires(T renderer, detail::ScreenArchetype& screen, Scene& scene) {
    { renderer.renderScene(scene, screen) } -> std::same_as<void>;
};
} // namespace cg
