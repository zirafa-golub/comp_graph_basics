#include "ray_tracer/RayTraceRenderer.h"

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

        // Compute direct illumination from lights while checking if the spot is in a shadow
        for (const auto& light : scene.lights()) {
            Light::DistanceDesc lightDistance = light->distanceFrom(hitPoint);
            Ray rayToLight(hitPoint + raySurfaceOffset * hit.unitNormal, lightDistance.unitDirection);

            auto shadowResult = scene.hit(rayToLight, 0, lightDistance.distance);
            if (!shadowResult.has_value()) {
                Color reflectedLight = hit.hitShape->material().reflect(hit.unitNormal, hit.unitViewDirection,
                                                                        lightDistance.unitDirection);
                pixelColor += reflectedLight * light->illuminate(hit);
            }
        }

        // Compute light reflected form other objects
        if (hit.hitShape->material().surfaceReflectance() != Color::black() && currBounceCount < maxBounces_) {
            auto reflectedDirection = glm::reflect(hit.ray.direction(), hit.unitNormal);
            Ray reflectedRay(hitPoint + raySurfaceOffset * hit.unitNormal, reflectedDirection);
            pixelColor +=
                shadeRay(scene, reflectedRay, currBounceCount + 1) * hit.hitShape->material().surfaceReflectance();
        }

        // Ambient light
        pixelColor += scene.ambientLight() * hit.hitShape->ambientReflectance();
    }
    return pixelColor;
}

unsigned RayTraceRenderer::maxBounces() const { return maxBounces_; }
void RayTraceRenderer::setMaxBounces(unsigned newMaxBounces) { maxBounces_ = newMaxBounces; }
} // namespace cg
