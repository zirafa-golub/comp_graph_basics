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

    bool shouldCull(const Point& p1, const Point& p2, const Point& p3) {
        glm::vec3 faceNormal = glm::cross(p2 - p1, p3 - p1);
        return shouldCull(p1, faceNormal);
    }

    bool shouldCull(const Point& pointOnFace, const glm::vec3& faceNormal) {
        auto dotp = glm::dot(pointOnFace - cameraPosition_, faceNormal);
        return dotp >= 0;
    }

private:
    const Point& cameraPosition_;
};
} // namespace cg
