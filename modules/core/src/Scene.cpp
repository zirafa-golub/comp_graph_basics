#include "core/Scene.h"

namespace cg {

void Scene::addShape(std::unique_ptr<Shape>&& shape) { shapes_.push_back(std::move(shape)); }

const std::vector<std::unique_ptr<Shape>>& Scene::shapes(std::unique_ptr<Shape>&& shape) const { return shapes_; }

std::vector<Shape*> Scene::shapes(std::unique_ptr<Shape>&& shape) {
    std::vector<Shape*> shapes;
    for (auto& shape : shapes_) {
        shapes.push_back(shape.get());
    }
    return shapes;
}

void Scene::setCamera(std::unique_ptr<Camera>&& camera) { camera_ = std::move(camera); }

const Camera& Scene::camera() const { return *camera_; }

cg::Camera& Scene::camera() { return *camera_; }

} // namespace cg