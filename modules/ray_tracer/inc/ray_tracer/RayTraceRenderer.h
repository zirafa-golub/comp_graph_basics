#pragma once

#include "core/Scene.h"
#include "screen/SdlScreen.h"

#include <limits>

namespace cg {
class RayTraceRenderer {
public:
    void renderScene(const Scene& scene, SdlScreen& screen);

    unsigned maxBounces() const;
    void setMaxBounces(unsigned newMaxBounces);

private:
    Color shadeRay(const Scene& scene, const Ray& ray, unsigned currBounceCount = 0);

    static constexpr float raySurfaceOffset = 0.00001f;

    unsigned maxBounces_ = 5;
};
} // namespace cg
