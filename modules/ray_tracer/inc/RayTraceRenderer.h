#pragma once

#include "core/Scene.h"
#include "SdlScreen.h"

namespace cg {
class RayTraceRenderer {
public:
    void renderScene(const Scene& scene, SdlScreen& screen);

private:
    static constexpr float shadowRayOffset = 0.00001f;
};
} // namespace cg
