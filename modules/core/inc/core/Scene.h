#pragma once

#include "core/BasicTypes.h"
#include "core/Camera.h"
#include "core/Shape.h"

#include <memory>
#include <vector>

namespace cg {
class Scene {
public:
    void addShape(std::unique_ptr<Shape>&& shape);
    const std::vector<std::unique_ptr<Shape>>& shapes(std::unique_ptr<Shape>&& shape) const;
    std::vector<Shape*> shapes(std::unique_ptr<Shape>&& shape);

    void setCamera(std::unique_ptr<Camera>&& camera);
    const std::unique_ptr<Camera>& camera() const;
    cg::Camera& camera();

private:
    std::vector<std::unique_ptr<Shape>> shapes_;
    std::unique_ptr<Camera> camera_;
};
} // namespace cg