#pragma once

#include "core/Scene.h"
#include "screen/SdlScreen.h"

#include <limits>

namespace cg {
struct HitDesc;

class RayTraceRenderer {
public:
    void renderScene(Scene& scene, SdlScreen& screen);

    unsigned maxBounces() const;
    void setMaxBounces(unsigned newMaxBounces);

private:
    Color shadeRay(Scene& scene, const Ray& ray, unsigned currBounceCount);
    std::optional<HitDesc> hitScene(const Ray& ray, float rayMin, float rayMax);

    static constexpr float raySurfaceOffset = 0.00005f;

    unsigned maxBounces_ = 5;
    std::vector<Shape*> sceneShapes_;
};
} // namespace cg
