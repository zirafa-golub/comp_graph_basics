#pragma once

#include "core/Mesh.h"
#include "core/ShaderGroup.h"
#include "core/Shape.h"
#include "core/Sphere.h"

#include <concepts>
#include <memory>

namespace cg {
namespace detail {
template <typename FactoryType>
concept HasShaderGroupCreateType = requires {
    { FactoryType::create() } -> std::same_as<std::unique_ptr<ShaderGroup>>;
};
} // namespace detail

template <template <typename T> typename FactoryType>
concept IsShaderGroupFactory =
    detail::HasShaderGroupCreateType<FactoryType<Sphere>> && detail::HasShaderGroupCreateType<FactoryType<Mesh>>;

struct ShapeFactory {
    template <typename ShapeType, template <typename T> typename ShaderGroupFactory, typename... ShapeArgs>
        requires std::derived_from<ShapeType, Shape> && IsShaderGroupFactory<ShaderGroupFactory>
    static std::unique_ptr<ShapeType> create(ShapeArgs&&... shapeArgs) {
        auto shape = std::make_unique<ShapeType>(std::forward<ShapeArgs>(shapeArgs)...);
        shape->setShaderGroup(ShaderGroupFactory<ShapeType>::create());
        return shape;
    }
};

} // namespace cg
