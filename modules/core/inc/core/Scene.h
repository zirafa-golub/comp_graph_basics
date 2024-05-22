#pragma once

#include "core/BasicTypes.h"
#include "core/Camera.h"
#include "core/Light.h"
#include "core/Shape.h"

#include <memory>
#include <vector>

namespace cg {
class Scene {
public:
    void addShape(std::unique_ptr<Shape>&& shape);
    const std::vector<std::unique_ptr<Shape>>& shapes() const;
    std::vector<Shape*> shapes();

    void addLight(std::unique_ptr<Light>&& light);
    const std::vector<std::unique_ptr<Light>>& lights() const;
    std::vector<Light*> lights();

    void setCamera(std::unique_ptr<Camera>&& camera);
    const Camera& camera() const;
    cg::Camera& camera();

    const Color& ambientLight() const;
    void setAmbientLight(const Color& newAmbient);

private:
    std::vector<std::unique_ptr<Shape>> shapes_;
    std::vector<std::unique_ptr<Light>> lights_;
    std::unique_ptr<Camera> camera_;
    Color ambientLight_;
};
} // namespace cg
