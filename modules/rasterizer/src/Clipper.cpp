#include "rasterizer/Clipper.h"

#include "common/Math.h"

#include <cassert>
#include <cmath>

namespace cg {
FrustumIntersect::FrustumIntersect(const PerspectiveCamera::FrustumPoints& frustum)
    : nearPlane_(frustum.nlb, frustum.nrb, frustum.nlt), farPlane_(frustum.frb, frustum.flb, frustum.flt),
      leftPlane_(frustum.flb, frustum.nlb, frustum.nlt), rightPlane_(frustum.nrb, frustum.frb, frustum.frt),
      topPlane_(frustum.flt, frustum.nlt, frustum.nrt), bottomPlane_(frustum.flb, frustum.frb, frustum.nrb) {}

FrustumIntersect::Planes FrustumIntersect::planesBelowVertex(const Point& vertex, Planes planesToCheck) const {
    Planes planesBelow = NoPlanes;

    if (planesToCheck & Plane::Near && nearPlane_.isVertexAbove(vertex)) {
        planesBelow |= Plane::Near;
    }
    if (planesToCheck & Plane::Far && farPlane_.isVertexAbove(vertex)) {
        planesBelow |= Plane::Far;
    }
    if (planesToCheck & Plane::Left && leftPlane_.isVertexAbove(vertex)) {
        planesBelow |= Plane::Left;
    }
    if (planesToCheck & Plane::Right && rightPlane_.isVertexAbove(vertex)) {
        planesBelow |= Plane::Right;
    }
    if (planesToCheck & Plane::Top && topPlane_.isVertexAbove(vertex)) {
        planesBelow |= Plane::Top;
    }
    if (planesToCheck & Plane::Bottom && bottomPlane_.isVertexAbove(vertex)) {
        planesBelow |= Plane::Bottom;
    }
    return planesBelow;
}

std::optional<Point> FrustumIntersect::findIntersectionPoint(const Point& insideVertex, const Point& outsideVertex,
                                                             Planes planesToCheck) const {
    float resultDistance = std::numeric_limits<float>::max();
    std::optional<Point> result = std::nullopt;

    if (planesToCheck & Plane::Near) {
        auto candidate = nearPlane_.intersectSegment(insideVertex, outsideVertex);
        assert(candidate.has_value());
        if (planesToCheck == Plane::Near) {
            return candidate;
        }
        float candidateDistance = glm::length(candidate.value() - insideVertex);
        if (candidateDistance < resultDistance) {
            resultDistance = candidateDistance;
            result = candidate;
        }
    }
    if (planesToCheck & Plane::Far) {
        auto candidate = farPlane_.intersectSegment(insideVertex, outsideVertex);
        assert(candidate.has_value());
        if (planesToCheck == Plane::Far) {
            return candidate;
        }
        float candidateDistance = glm::length(candidate.value() - insideVertex);
        if (candidateDistance < resultDistance) {
            resultDistance = candidateDistance;
            result = candidate;
        }
    }
    if (planesToCheck & Plane::Left) {
        auto candidate = leftPlane_.intersectSegment(insideVertex, outsideVertex);
        assert(candidate.has_value());
        if (planesToCheck == Plane::Left) {
            return candidate;
        }
        float candidateDistance = glm::length(candidate.value() - insideVertex);
        if (candidateDistance < resultDistance) {
            resultDistance = candidateDistance;
            result = candidate;
        }
    }
    if (planesToCheck & Plane::Right) {
        auto candidate = rightPlane_.intersectSegment(insideVertex, outsideVertex);
        assert(candidate.has_value());
        if (planesToCheck == Plane::Right) {
            return candidate;
        }
        float candidateDistance = glm::length(candidate.value() - insideVertex);
        if (candidateDistance < resultDistance) {
            resultDistance = candidateDistance;
            result = candidate;
        }
    }
    if (planesToCheck & Plane::Top) {
        auto candidate = topPlane_.intersectSegment(insideVertex, outsideVertex);
        assert(candidate.has_value());
        if (planesToCheck == Plane::Top) {
            return candidate;
        }
        float candidateDistance = glm::length(candidate.value() - insideVertex);
        if (candidateDistance < resultDistance) {
            resultDistance = candidateDistance;
            result = candidate;
        }
    }
    if (planesToCheck & Plane::Bottom) {
        auto candidate = bottomPlane_.intersectSegment(insideVertex, outsideVertex);
        assert(candidate.has_value());
        if (planesToCheck == Plane::Bottom) {
            return candidate;
        }
        float candidateDistance = glm::length(candidate.value() - insideVertex);
        if (candidateDistance < resultDistance) {
            resultDistance = candidateDistance;
            result = candidate;
        }
    }

    return result;
}

bool FrustumIntersect::isBelowPlanes(const Point& vertex, Planes planesToCheck) const {
    if (planesToCheck & Plane::Near && nearPlane_.isVertexAbove(vertex)) {
        return false;
    }
    if (planesToCheck & Plane::Far && farPlane_.isVertexAbove(vertex)) {
        return false;
    }
    if (planesToCheck & Plane::Left && leftPlane_.isVertexAbove(vertex)) {
        return false;
    }
    if (planesToCheck & Plane::Right && rightPlane_.isVertexAbove(vertex)) {
        return false;
    }
    if (planesToCheck & Plane::Top && topPlane_.isVertexAbove(vertex)) {
        return false;
    }
    if (planesToCheck & Plane::Bottom && bottomPlane_.isVertexAbove(vertex)) {
        return false;
    }
    return true;
}

Clipper::Clipper(const FrustumIntersect& frustum) : frustum_(frustum), inputMesh_(nullptr) {}

void Clipper::initForMesh(const MeshData& mesh) {
    planesBelowVertex_.resize(mesh.vertices().size(), FrustumIntersect::NoPlanes);
    copiedVertexIndex_.resize(mesh.vertices().size(), notCopiedIndex);

    size_t vertexCount = mesh.vertices().size();
    outputVertices_.reserve(vertexCount);
    outputTriangles_.reserve(mesh.triangles().size());
    inputMesh_ = &mesh;
}

void Clipper::cleanUp() {
    inputMesh_ = nullptr;
    planesBelowVertex_.clear();
    copiedVertexIndex_.clear();
}

MeshData Clipper::clip(const MeshData& mesh) {
    initForMesh(mesh);

    populateVertexFrustumInfo();
    for (const auto& triangle : inputMesh_->triangles()) {
        clipTriangle(triangle);
    }

    MeshData result(std::move(outputVertices_), std::move(outputTriangles_));
    cleanUp();
    return result;
}

void Clipper::populateVertexFrustumInfo() {
    const auto& vertices = inputMesh_->vertices();
    for (unsigned i = 0; i < vertices.size(); ++i) {
        planesBelowVertex_[i] = frustum_.planesBelowVertex(vertices[i]);
    }
}

void Clipper::clipTriangle(const MeshData::TriangleIndices& triangle) {
    const auto& vertices = inputMesh_->vertices();
    FrustumIntersect::Planes verticesOutsideFrustum = (planesBelowVertex_[triangle[0]] != FrustumIntersect::NoPlanes) +
                                                      (planesBelowVertex_[triangle[1]] != FrustumIntersect::NoPlanes) +
                                                      (planesBelowVertex_[triangle[2]] != FrustumIntersect::NoPlanes);
    if (verticesOutsideFrustum == 0) {
        // Triangle is fully inside frustum - put in output without changes
        MeshData::TriangleIndices updatedTriangle;
        for (int i = 0; i < triangle.size(); ++i) {
            updatedTriangle[i] = copyVertexIfNeeded(triangle[i]);
        }
        outputTriangles_.push_back(updatedTriangle);
    } else if (verticesOutsideFrustum == 1) {
        // Triangle is cut so quad is left on the inside
        FrustumIntersect::Planes planesBelowVertex;
        unsigned i = 0;
        for (; i < triangle.size(); ++i) {
            planesBelowVertex = planesBelowVertex_[triangle[i]];
            if (planesBelowVertex != FrustumIntersect::NoPlanes) {
                break;
            }
        }

        unsigned insideVertex1Index = triangle[(i + 1) % triangle.size()];
        unsigned insideVertex2Index = triangle[(i + 2) % triangle.size()];
        const Point& outsideVertex = vertices[triangle[i]];
        const Point& insideVertex1 = vertices[insideVertex1Index];
        const Point& insideVertex2 = vertices[insideVertex2Index];

        auto intersection1 = frustum_.findIntersectionPoint(insideVertex1, outsideVertex, planesBelowVertex);
        assert(intersection1.has_value());
        // TODO: avoid adding the same new vertex twice for the same edge but different trianges
        unsigned cutoffVertex1Index = addVertex(intersection1.value());

        auto intersection2 = frustum_.findIntersectionPoint(insideVertex2, outsideVertex, planesBelowVertex);
        assert(intersection2.has_value());
        unsigned cutoffVertex2Index = addVertex(intersection2.value());
        unsigned newInsideVertex1Index = copyVertexIfNeeded(insideVertex1Index);
        unsigned newInsideVertex2Index = copyVertexIfNeeded(insideVertex2Index);

        outputTriangles_.push_back({cutoffVertex1Index, newInsideVertex1Index, newInsideVertex2Index});
        outputTriangles_.push_back({cutoffVertex1Index, newInsideVertex2Index, cutoffVertex2Index});
    } else if (verticesOutsideFrustum == 2) {
        // Triangle is cut so triangle is left on the inside
        int i = 0;
        for (; i < triangle.size(); ++i) {
            if (planesBelowVertex_[triangle[i]] == FrustumIntersect::NoPlanes) {
                break;
            }
        }
        unsigned insideVertex1Index = triangle[i];
        unsigned outsideVertex1Index = triangle[(i + 1) % triangle.size()];
        unsigned outsideVertex2Index = triangle[(i + 2) % triangle.size()];
        const Point& insideVertex = vertices[insideVertex1Index];
        const Point& outsideVertex1 = vertices[triangle[(i + 1) % triangle.size()]];
        const Point& outsideVertex2 = vertices[triangle[(i + 2) % triangle.size()]];

        auto intersection1 =
            frustum_.findIntersectionPoint(insideVertex, outsideVertex1, planesBelowVertex_[outsideVertex1Index]);
        assert(intersection1.has_value());
        // TODO: avoid adding the same new vertex twice for the same edge but different trianges
        unsigned cutoffVertex1Index = addVertex(intersection1.value());

        auto intersection2 =
            frustum_.findIntersectionPoint(insideVertex, outsideVertex2, planesBelowVertex_[outsideVertex2Index]);
        assert(intersection2.has_value());
        unsigned cutoffVertex2Index = addVertex(intersection2.value());

        unsigned newInsideVertex = copyVertexIfNeeded(insideVertex1Index);

        outputTriangles_.push_back({newInsideVertex, cutoffVertex1Index, cutoffVertex2Index});
    }
    // TODO? support case where all vertices are outside frustum, but they still intersect with it (e.g. they are on
    // oposite sides of frustum)

    // If triangle is fully ouside the frustum, don't move anything to the output
}

unsigned Clipper::copyVertexIfNeeded(unsigned inputVertexIndex) {
    const auto& vertices = inputMesh_->vertices();
    unsigned vertexIndexInOutput = copiedVertexIndex_[inputVertexIndex];
    if (vertexIndexInOutput == notCopiedIndex) {
        vertexIndexInOutput = static_cast<unsigned>(outputVertices_.size());
        copiedVertexIndex_[inputVertexIndex] = vertexIndexInOutput;
        outputVertices_.push_back(vertices[inputVertexIndex]);
    }

    return vertexIndexInOutput;
}

unsigned Clipper::addVertex(const Point& vertex) {
    unsigned newVertexIndex = static_cast<unsigned>(outputVertices_.size());
    outputVertices_.push_back(vertex);
    return newVertexIndex;
}

} // namespace cg
