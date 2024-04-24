#pragma once

#include "core/Angle.h"
#include "core/BasicTypes.h"
#include "core/MeshData.h"
#include "core/PerspectiveCamera.h"
#include "rasterizer/PlaneIntersect.h"

#include <limits>
#include <optional>
#include <type_traits>
#include <vector>

namespace cg {
class FrustumIntersect {
public:
    enum Plane : uint8_t {
        Near = 0x01,
        Far = 0x02,
        Left = 0x04,
        Right = 0x08,
        Top = 0x10,
        Bottom = 0x20,
    };
    using Planes = std::underlying_type_t<Plane>;
    static constexpr Planes AllPlanes = 0x3F;
    static constexpr Planes NoPlanes = 0x00;

    FrustumIntersect(const PerspectiveCamera::FrustumPoints& frustum);
    Planes planesBelowVertex(const Point& vertex, Planes planesToCheck = AllPlanes) const;
    std::optional<Point> findIntersectionPoint(const Point& v1, const Point& v2, Planes planesToCheck) const;

private:
    bool isBelowPlanes(const Point& vertex, Planes planesToCheck) const;

    PlaneIntersect nearPlane_;
    PlaneIntersect farPlane_;
    PlaneIntersect leftPlane_;
    PlaneIntersect rightPlane_;
    PlaneIntersect topPlane_;
    PlaneIntersect bottomPlane_;
};

class Clipper {
public:
    Clipper(const FrustumIntersect& frustum);
    MeshData clip(const MeshData& mesh);

private:
    void initForMesh(const MeshData& mesh);
    void cleanUp();
    void populateVertexFrustumInfo();
    void clipTriangle(const TriangleIndices& triangle);
    unsigned copyVertexIfNeeded(unsigned inputVertexIndex);
    unsigned addVertex(const Point& vertex);

    static constexpr unsigned notCopiedIndex = std::numeric_limits<unsigned>::max();

    const FrustumIntersect& frustum_;

    const MeshData* inputMesh_;
    std::vector<FrustumIntersect::Planes> planesBelowVertex_;
    std::vector<unsigned> copiedVertexIndex_;

    std::vector<Point> outputVertices_;
    std::vector<TriangleIndices> outputTriangles_;
};
} // namespace cg
