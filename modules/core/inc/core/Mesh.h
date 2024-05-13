#pragma once

#include "core/MeshData.h"
#include "core/Shape.h"

#include <array>

namespace cg {

class Mesh : public Shape {
public:
    struct TriangleHit {
        float tHit;
        float beta;
        float gamma;
    };

    Mesh(MeshData meshData);

    std::expected<HitDesc, Error> hit(const Ray& ray, float tMin, float tMax) const override;
    static std::expected<TriangleHit, ErrorCode> hitTriangle(const Ray& ray, float tMin, float tMax,
                                                             const Point& vertexA, const Point& vertexB,
                                                             const Point& vertexC);
    const MeshData& meshData() const override;

protected:
    void transformUpdated() override;

private:
    MeshData meshData_;
    glm::mat3 transposedLocalFrame_;
};
} // namespace cg
