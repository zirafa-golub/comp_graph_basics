#pragma once

#include "core/BasicTypes.h"
#include "core/MeshData.h"
#include "core/Shape.h"

#include "glm/mat4x4.hpp"

#include <array>
#include <cmath>
#include <expected>

namespace cg {
class Angle;
class Sphere : public Shape {
public:
    Sphere(float radius);

    const Point& center() const;

    float radius() const;

    const MeshData& meshData() const override;
    void generateMesh(unsigned verticalSegments, unsigned horizontalSegments);

private:
    Point generatePoint(Angle verticalAngle, Angle horizontalAngle) const;

    float radius_;
    std::unique_ptr<MeshData> meshData_;
};
} // namespace cg
