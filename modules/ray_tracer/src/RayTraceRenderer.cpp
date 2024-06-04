#include "ray_tracer/RayTraceRenderer.h"

#include <limits>

namespace cg {
Color RayTraceRenderer::shadeRay(Scene& scene, const Ray& ray, unsigned currBounceCount) {
    Color pixelColor = Color(0, 0, 0);
    auto result = hitScene(ray, 0, std::numeric_limits<float>::infinity());
    if (result.has_value()) {
        const HitDesc& hit = result.value();
        Point hitPoint = hit.ray.evaluate(hit.rayHitVal);

        // Compute direct illumination from lights while checking if the spot is in a shadow
        for (const auto& light : scene.lights()) {
            Light::DistanceDesc lightDistance = light->distanceFrom(hitPoint);
            Ray rayToLight(hitPoint + raySurfaceOffset * hit.unitNormal, lightDistance.unitDirection);

            auto shadowResult = hitScene(rayToLight, 0, lightDistance.distance);
            if (!shadowResult.has_value()) {
                Color reflectedLight = hit.hitShape->material().reflect(hit.unitNormal, hit.unitViewDirection,
                                                                        lightDistance.unitDirection);
                pixelColor += reflectedLight * light->illuminate(hitPoint, hit.unitNormal);
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

std::optional<HitDesc> RayTraceRenderer::hitScene(const Ray& ray, float rayMin, float rayMax) {
    std::optional<HitDesc> result = std::nullopt;
    for (auto shape : sceneShapes_) {
        RayTracerShaders& shaderGroup = static_cast<RayTracerShaders&>(shape->shaderGroup());
        auto hitResult = shaderGroup.hitDetector().hit(ray, rayMin, rayMax);
        if (hitResult.has_value()) {
            result = std::move(hitResult);
            rayMax = result.value().rayHitVal;
        }
    }

    return result;
}

unsigned RayTraceRenderer::maxBounces() const { return maxBounces_; }
void RayTraceRenderer::setMaxBounces(unsigned newMaxBounces) { maxBounces_ = newMaxBounces; }
} // namespace cg
