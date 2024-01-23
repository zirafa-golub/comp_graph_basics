#include "RayTraceRenderer.h"

#include <limits>

namespace cg {

void RayTraceRenderer::renderScene(const Scene& scene, SdlScreen& screen) {
    const Camera& camera = scene.camera();
    auto res = camera.resolution();
    SdlScreen::Painter painter = screen.paintPixels();

    for (unsigned row = 0; row < res.height; ++row) {
        for (unsigned col = 0; col < res.width; ++col) {
            Color pixelColor = Color(0, 0, 0);
            auto result = scene.hit(camera.castRay(col, row), 0.0f, std::numeric_limits<float>::infinity());
            if (result.has_value()) {
                for (const auto& light : scene.lights()) {
                    pixelColor += light->illuminate(result.value());
                }
            }
            painter.paintPixel(row, col, pixelColor);
        }
    }
}
} // namespace cg
