#pragma once

#include "core/BasicTypes.h"
#include "core/MeshData.h"

namespace cg {
class Angle;

class MeshGenerator {
public:
    static MeshData generateSphere(float radius, unsigned verticalSegCount, unsigned horizontalSegCount);
    static MeshData generateRectangle(Size2d size, unsigned horSegCount, unsigned verSegCount);

private:
    static Point generateSpherePoint(float radius, Angle verticalAngle, Angle horizontalAngle);
};
} // namespace cg
