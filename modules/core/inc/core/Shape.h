#pragma once

#include "core/BasicTypes.h"
#include "core/Color.h"
#include "core/Material.h"
#include "core/MeshData.h"
#include "core/ShaderGroup.h"
#include "core/Transformable.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

namespace cg {

class Shape : public Transformable {
public:
    virtual const MeshData& meshData() const = 0;

    const Material& material() const { return *material_; }
    Material& material() { return *material_; }
    void setMaterial(std::unique_ptr<Material> material) { material_ = std::move(material); }

    const Color& ambientReflectance() const { return ambientReflectance_; }
    void setAmbientReflectance(const Color& newReflectance) { ambientReflectance_ = newReflectance; }

    const ShaderGroup& shaderGroup() const {
        assert(shaderGroup_ != nullptr);
        return *shaderGroup_;
    }
    ShaderGroup& shaderGroup() {
        assert(shaderGroup_ != nullptr);
        return *shaderGroup_;
    }
    void setShaderGroup(std::unique_ptr<ShaderGroup>&& shaderGroup) {
        assert(shaderGroup != nullptr);

        shaderGroup->setShape(*this);
        shaderGroup_ = std::move(shaderGroup);
    }

protected:
    Shape() = default;

private:
    std::unique_ptr<Material> material_;
    Color ambientReflectance_ = Color(1, 1, 1);
    std::unique_ptr<ShaderGroup> shaderGroup_;
};
} // namespace cg
