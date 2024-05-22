#include "hit/MeshHitDetector.h"
#include "hit/SphereHitDetector.h"
#include "ray_tracer/RayTracerShaders.h"
#include "RayTracerShaderFactory.h"

#include <memory>

namespace cg {
std::unique_ptr<ShaderGroup> RayTracerShaderFactory::createSphereShaders() const {
    return std::make_unique<RayTracerShaders>(std::make_unique<SphereHitDetector>());
}
std::unique_ptr<ShaderGroup> RayTracerShaderFactory::createMeshShaders() const {
    return std::make_unique<RayTracerShaders>(std::make_unique<MeshHitDetector>());
}
} // namespace cg
