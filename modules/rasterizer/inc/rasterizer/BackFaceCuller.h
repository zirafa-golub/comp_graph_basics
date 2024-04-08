#pragma once

#include "common/Math.h"
#include "core/BasicTypes.h"
#include "core/MeshData.h"

#include "glm/geometric.hpp"
#include "glm/vec3.hpp"

#include <cassert>
#include <vector>

namespace cg {
class BackFaceCuller {
public:
    BackFaceCuller(const Point& cameraPosition) : cameraPosition_(cameraPosition) {}

    bool shouldCull(const std::vector<Point>& points, const MeshData::TriangleIndices& triangle) {
        glm::vec3 faceNormal =
            glm::cross(points[triangle[1]] - points[triangle[0]], points[triangle[2]] - points[triangle[0]]);
        return shouldCull(points[triangle[0]], faceNormal);
    }

    bool shouldCull(const Point& pointOnFace, const glm::vec3& faceNormal) {
        auto dotp = glm::dot(pointOnFace - cameraPosition_, faceNormal);
        return dotp >= 0;
    }

private:
    const Point& cameraPosition_;
};
} // namespace cg
