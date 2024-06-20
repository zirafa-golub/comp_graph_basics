#pragma once

#include "core/Scene.h"
#include "ray_tracer/RayTracerShaders.h"
#include "renderer/Renderer.h"
#include "task/TaskGraph.h"

#include <limits>

namespace cg {
struct HitDesc;

class RayTraceRenderer {
public:
    void renderScene(Scene& scene, Screen auto& screen) {
        sceneShapes_ = scene.shapes();

        for (auto shape : sceneShapes_) {
            RayTracerShaders& shaderGroup = static_cast<RayTracerShaders&>(shape->shaderGroup());
            shaderGroup.hitDetector().initForFrame();
        }

        const Camera& camera = scene.camera();
        auto res = camera.resolution();

        TaskBatch batch;
        for (unsigned row = 0; row < res.height; ++row) {
            batch.addWork([this, &camera, &scene, painter = screen.paintPixels(), row, width = res.width]() mutable {
                for (unsigned col = 0; col < width; ++col) {
                    Color pixelColor = shadeRay(scene, camera.castRay(col, row), 0);
                    painter.paint(row, col, pixelColor);
                }
            });
        }
        batch.startAndWait(threadPool);
    }

    unsigned maxBounces() const;
    void setMaxBounces(unsigned newMaxBounces);

private:
    Color shadeRay(Scene& scene, const Ray& ray, unsigned currBounceCount) const;
    std::optional<HitDesc> hitScene(const Ray& ray, float rayMin, float rayMax) const;

    static constexpr float raySurfaceOffset = 0.00005f;

    unsigned maxBounces_ = 5;
    std::vector<Shape*> sceneShapes_;
    ThreadPool threadPool;
};

static_assert(Renderer<RayTraceRenderer>, "RayTraceRenderer does not fulfill the Renderer concept.");
} // namespace cg
