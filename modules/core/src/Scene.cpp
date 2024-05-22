#include "core/Scene.h"

namespace cg {

void Scene::addShape(std::unique_ptr<Shape>&& shape) { shapes_.push_back(std::move(shape)); }

const std::vector<std::unique_ptr<Shape>>& Scene::shapes() const { return shapes_; }

std::vector<Shape*> Scene::shapes() {
    std::vector<Shape*> shapes;
    shapes.reserve(shapes_.size());
    for (auto& shape : shapes_) {
        shapes.push_back(shape.get());
    }
    return shapes;
}

void Scene::addLight(std::unique_ptr<Light>&& light) { lights_.push_back(std::move(light)); }

const std::vector<std::unique_ptr<Light>>& Scene::lights() const { return lights_; }

std::vector<Light*> Scene::lights() {
    std::vector<Light*> lights;
    lights.reserve(lights_.size());
    for (auto& light : lights_) {
        lights.push_back(light.get());
    }
    return lights;
}

void Scene::setCamera(std::unique_ptr<Camera>&& camera) { camera_ = std::move(camera); }

const Camera& Scene::camera() const { return *camera_; }

cg::Camera& Scene::camera() { return *camera_; }

const Color& Scene::ambientLight() const { return ambientLight_; }

void Scene::setAmbientLight(const Color& newAmbient) { ambientLight_ = newAmbient; }
} // namespace cg
