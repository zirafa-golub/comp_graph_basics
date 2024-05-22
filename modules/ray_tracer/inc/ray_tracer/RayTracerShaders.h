#pragma once

#include "core/ShaderGroup.h"
#include "hit/HitDetector.h"

#include <memory>

namespace cg {

class RayTracerShaders : public ShaderGroup {
public:
    RayTracerShaders(std::unique_ptr<HitDetector> hitDetector) : ShaderGroup(), hitDetector_(std::move(hitDetector)) {
        hitDetector_->setShaderGroup(*this);
    }

    HitDetector& hitDetector() { return *hitDetector_; }

private:
    std::unique_ptr<HitDetector> hitDetector_;
};
} // namespace cg
