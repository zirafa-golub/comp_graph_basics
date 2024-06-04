#pragma once

#include "core/ShaderGroup.h"
#include "shader/ShapeShader.h"

#include <memory>

namespace cg {
class RasterizerShaders : public ShaderGroup {
public:
    RasterizerShaders(std::unique_ptr<ShapeShader>&& shapeShader) : shapeShader_(std::move(shapeShader)) {}

    const ShapeShader& shapeShader() const { return *shapeShader_; }

private:
    std::unique_ptr<ShapeShader> shapeShader_;
};
} // namespace cg
