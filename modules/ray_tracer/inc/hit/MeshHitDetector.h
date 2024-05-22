#pragma once

#include "hit/HitDetector.h"

namespace cg {
class MeshData;
class Mesh;

class MeshHitDetector : public HitDetector {
public:
    struct TriangleHit {
        float rayHitVal;
        float beta;
        float gamma;
    };

    void initForFrame() override;
    std::optional<HitDesc> hit(const Ray& ray, float rayMin, float rayMax) const override;

    static std::optional<TriangleHit> hitTriangle(const Ray& ray, float rayMin, float rayMax, const Point& vertexA,
                                                  const Point& vertexB, const Point& vertexC);

private:
    const Mesh* mesh_;
    std::vector<Point> transformedVertices_;
    std::vector<glm::vec3> transformedNormals_;
};
} // namespace cg
