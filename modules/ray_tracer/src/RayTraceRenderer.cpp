#include "RayTraceRenderer.h"

#include <limits>

#include "core/Sphere.h"

namespace cg {

void RayTraceRenderer::renderScene(const Scene& scene, SdlScreen& screen) {
    const Camera& camera = scene.camera();
    auto res = camera.resolution();
    SdlScreen::Painter painter = screen.paintPixels();

    for (unsigned row = 0; row < res.height; ++row) {
        for (unsigned col = 0; col < res.width; ++col) {
            Color pixelColor = shadeRay(scene, camera.castRay(col, row), 0);
            painter.paintPixel(row, col, pixelColor);
        }
    }
}

Color RayTraceRenderer::shadeRay(const Scene& scene, const Ray& ray, unsigned currBounceCount) {
    Color pixelColor = Color(0, 0, 0);
    auto result = scene.hit(ray, 0, std::numeric_limits<float>::infinity());
    if (result.has_value()) {
        const HitDesc& hit = result.value();
        Point hitPoint = hit.ray.evaluate(hit.tHit);

        for (const auto& light : scene.lights()) {
            // Check if spot is in shadow
            Light::DistanceDesc lightDistance = light->distanceFrom(hitPoint);
            if (lightDistance.distance == 0.0f) {
                pixelColor += light->illuminate(hit);
            } else {
                Ray rayToLight(hitPoint + raySurfaceOffset * hit.unitNormal, lightDistance.direction);
                auto shadowResult = scene.hit(rayToLight, 0, lightDistance.distance);
                if (!shadowResult.has_value()) {
                    pixelColor += light->illuminate(result.value());
                }
            }
        }
        if (hit.hitShape->material().surfaceReflectance() != Color::black() && currBounceCount < maxBounces_) {
            auto reflectedDirection = glm::reflect(hit.ray.direction(), hit.unitNormal);
            Ray reflectedRay(hitPoint + raySurfaceOffset * hit.unitNormal, reflectedDirection);
            pixelColor +=
                shadeRay(scene, reflectedRay, currBounceCount + 1) * hit.hitShape->material().surfaceReflectance();
        }
    }
    return pixelColor;
}

unsigned RayTraceRenderer::maxBounces() const { return maxBounces_; }
void RayTraceRenderer::setMaxBounces(unsigned newMaxBounces) { maxBounces_ = newMaxBounces; }
} // namespace cg
