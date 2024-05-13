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

std::optional<PlaneIntersect::SegmentIntersection> FrustumIntersect::findIntersectionPoint(const Point& insideVertex,
                                                                                           const Point& outsideVertex,
                                                                                           Planes planesToCheck) const {
    float resultDistance = std::numeric_limits<float>::max();
    std::optional<PlaneIntersect::SegmentIntersection> result = std::nullopt;

    if (planesToCheck & Plane::Near) {
        auto candidate = nearPlane_.intersectSegment(insideVertex, outsideVertex);
        assert(candidate.has_value());
        if (planesToCheck == Plane::Near) {
            return candidate;
        }
        float candidateDistance = candidate.value().fromFirstToSecond;
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
        float candidateDistance = candidate.value().fromFirstToSecond;
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
        float candidateDistance = candidate.value().fromFirstToSecond;
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
        float candidateDistance = candidate.value().fromFirstToSecond;
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
        float candidateDistance = candidate.value().fromFirstToSecond;
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
        float candidateDistance = candidate.value().fromFirstToSecond;
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
    size_t vertexCount = mesh.vertices().size();
    size_t vertexNormalCount = mesh.vertexNormals().size();
    planesBelowVertex_.resize(vertexCount, FrustumIntersect::NoPlanes);
    copiedVertexIndex_.resize(vertexCount, notCopiedIndex);
    copiedVertexNormalIndex_.resize(vertexNormalCount, notCopiedIndex);

    outputVertices_.reserve(vertexCount);
    outputVertexNormals_.reserve(vertexNormalCount);
    outputTriangles_.reserve(mesh.triangles().size());
    inputMesh_ = &mesh;
}

void Clipper::cleanUp() {
    inputMesh_ = nullptr;
    planesBelowVertex_.clear();
    copiedVertexIndex_.clear();
    copiedVertexNormalIndex_.clear();
}

MeshData Clipper::clip(const MeshData& mesh) {
    initForMesh(mesh);

    populateVertexFrustumInfo();
    for (const auto& triangle : inputMesh_->triangles()) {
        clipTriangle(triangle);
    }

    MeshData result(std::move(outputVertices_), std::move(outputVertexNormals_), std::move(outputTriangles_));
    cleanUp();
    return result;
}

void Clipper::populateVertexFrustumInfo() {
    const auto& vertices = inputMesh_->vertices();
    for (unsigned i = 0; i < vertices.size(); ++i) {
        planesBelowVertex_[i] = frustum_.planesBelowVertex(vertices[i]);
    }
}

void Clipper::clipTriangle(const TriangleData& triangle) {
    FrustumIntersect::Planes verticesOutsideFrustum =
        (planesBelowVertex_[triangle[0].vertex] != FrustumIntersect::NoPlanes) +
        (planesBelowVertex_[triangle[1].vertex] != FrustumIntersect::NoPlanes) +
        (planesBelowVertex_[triangle[2].vertex] != FrustumIntersect::NoPlanes);

    if (verticesOutsideFrustum == 0) {
        // Triangle is fully inside frustum - add it to output without changes
        TriangleData updatedTriangle;
        for (int i = 0; i < triangle.size(); ++i) {
            updatedTriangle[i] = copyVertexAndNormalIfNeeded(triangle[i].vertex, triangle[i].vertexNormal);
        }
        outputTriangles_.push_back(updatedTriangle);
    } else if (verticesOutsideFrustum == 1) {
        // Triangle is cut so quad is left on the inside
        FrustumIntersect::Planes planesBelowVertex;
        unsigned i = 0;
        for (; i < triangle.size(); ++i) {
            planesBelowVertex = planesBelowVertex_[triangle[i].vertex];
            if (planesBelowVertex != FrustumIntersect::NoPlanes) {
                break;
            }
        }
        MeshData::Index insideVertex1Index = triangle[(i + 1) % triangle.size()].vertex;
        MeshData::Index insideNormal1Index = triangle[(i + 1) % triangle.size()].vertexNormal;
        MeshData::Index insideVertex2Index = triangle[(i + 2) % triangle.size()].vertex;
        MeshData::Index insideNormal2Index = triangle[(i + 2) % triangle.size()].vertexNormal;
        const auto& vertices = inputMesh_->vertices();
        const auto& vertexNormals = inputMesh_->vertexNormals();
        const Point& outsideVertex = vertices[triangle[i].vertex];
        const glm::vec3& outsideNormal = vertexNormals[triangle[i].vertexNormal];
        const Point& insideVertex1 = vertices[insideVertex1Index];
        const Point& insideVertex2 = vertices[insideVertex2Index];

        // Find first intersection
        auto intersectionResult = frustum_.findIntersectionPoint(insideVertex1, outsideVertex, planesBelowVertex);
        assert(intersectionResult.has_value());
        // TODO: avoid adding the same new vertex and normal twice for the same edge but different triangles
        auto& intersection1 = intersectionResult.value();
        glm::vec3 intersection1Normal =
            interpolateNormal(vertexNormals[insideNormal1Index], outsideNormal, intersection1.fromFirstToSecond);
        VertexData cutoffVertex1Data = addVertexAndNormal(intersection1.intersection, intersection1Normal);

        // Find second intersection
        intersectionResult = frustum_.findIntersectionPoint(insideVertex2, outsideVertex, planesBelowVertex);
        assert(intersectionResult.has_value());
        auto& intersection2 = intersectionResult.value();
        glm::vec3 intersection2Normal =
            interpolateNormal(vertexNormals[insideNormal2Index], outsideNormal, intersection2.fromFirstToSecond);
        VertexData cutoffVertex2Data = addVertexAndNormal(intersection2.intersection, intersection2Normal);

        // Copy over the inside vertices
        VertexData newInsideVertex1Data = copyVertexAndNormalIfNeeded(insideVertex1Index, insideNormal1Index);
        VertexData newInsideVertex2Data = copyVertexAndNormalIfNeeded(insideVertex2Index, insideNormal2Index);

        outputTriangles_.push_back({cutoffVertex1Data, newInsideVertex1Data, newInsideVertex2Data});
        outputTriangles_.push_back({cutoffVertex1Data, newInsideVertex2Data, cutoffVertex2Data});
    } else if (verticesOutsideFrustum == 2) {
        // Triangle is cut so triangle is left on the inside
        unsigned i = 0;
        for (; i < triangle.size(); ++i) {
            if (planesBelowVertex_[triangle[i].vertex] == FrustumIntersect::NoPlanes) {
                break;
            }
        }
        MeshData::Index insideVertexIndex = triangle[i].vertex;
        MeshData::Index insideNormalIndex = triangle[i].vertexNormal;
        MeshData::Index outsideVertex1Index = triangle[(i + 1) % triangle.size()].vertex;
        MeshData::Index outsideNormal1Index = triangle[(i + 1) % triangle.size()].vertexNormal;
        MeshData::Index outsideVertex2Index = triangle[(i + 2) % triangle.size()].vertex;
        MeshData::Index outsideNormal2Index = triangle[(i + 2) % triangle.size()].vertexNormal;
        const auto& vertices = inputMesh_->vertices();
        const auto& vertexNormals = inputMesh_->vertexNormals();
        const Point& insideVertex = vertices[insideVertexIndex];
        const glm::vec3& insideNormal = vertexNormals[triangle[i].vertexNormal];
        const Point& outsideVertex1 = vertices[outsideVertex1Index];
        const Point& outsideVertex2 = vertices[outsideVertex2Index];

        // Find first intersection
        auto intersectionResult =
            frustum_.findIntersectionPoint(insideVertex, outsideVertex1, planesBelowVertex_[outsideVertex1Index]);
        assert(intersectionResult.has_value());
        // TODO: avoid adding the same new vertex and normal twice for the same edge but different trianges
        auto& intersection1 = intersectionResult.value();
        glm::vec3 intersection1Normal =
            interpolateNormal(insideNormal, vertexNormals[outsideNormal1Index], intersection1.fromFirstToSecond);
        VertexData cutoffVertex1Data = addVertexAndNormal(intersection1.intersection, intersection1Normal);

        // Find second intersection
        intersectionResult =
            frustum_.findIntersectionPoint(insideVertex, outsideVertex2, planesBelowVertex_[outsideVertex2Index]);
        assert(intersectionResult.has_value());
        auto& intersection2 = intersectionResult.value();
        glm::vec3 intersection2Normal =
            interpolateNormal(insideNormal, vertexNormals[outsideNormal2Index], intersection2.fromFirstToSecond);
        VertexData cutoffVertex2Data = addVertexAndNormal(intersection2.intersection, intersection2Normal);

        // Copy over inside vertex
        VertexData newInsideVertex = copyVertexAndNormalIfNeeded(insideVertexIndex, insideNormalIndex);

        outputTriangles_.push_back({newInsideVertex, cutoffVertex1Data, cutoffVertex2Data});
    }
    // TODO? support case where all vertices are outside frustum, but they still intersect with it (e.g. they are on
    // oposite sides of frustum)

    // If triangle is fully ouside the frustum, don't move anything to the output
}

VertexData Clipper::copyVertexAndNormalIfNeeded(MeshData::Index inputVertexIndex,
                                                MeshData::Index inputVertexNormalIndex) {
    MeshData::Index vertexIndexInOutput = copiedVertexIndex_[inputVertexIndex];
    MeshData::Index normalIndexInOutput = copiedVertexNormalIndex_[inputVertexNormalIndex];
    if (vertexIndexInOutput == notCopiedIndex) {
        vertexIndexInOutput = static_cast<MeshData::Index>(outputVertices_.size());
        copiedVertexIndex_[inputVertexIndex] = vertexIndexInOutput;
        outputVertices_.push_back(inputMesh_->vertices()[inputVertexIndex]);

        if (normalIndexInOutput == notCopiedIndex) {
            normalIndexInOutput = static_cast<MeshData::Index>(outputVertexNormals_.size());
            copiedVertexNormalIndex_[inputVertexNormalIndex] = normalIndexInOutput;
            outputVertexNormals_.push_back(inputMesh_->vertexNormals()[inputVertexNormalIndex]);
        }
    }

    return {vertexIndexInOutput, normalIndexInOutput};
}

VertexData Clipper::addVertexAndNormal(const Point& vertex, const glm::vec3& vertexNormal) {
    MeshData::Index newVertexIndex = static_cast<MeshData::Index>(outputVertices_.size());
    outputVertices_.push_back(vertex);
    MeshData::Index newNormalIndex = static_cast<MeshData::Index>(outputVertexNormals_.size());
    outputVertexNormals_.push_back(vertexNormal);

    return {newVertexIndex, newNormalIndex};
}

glm::vec3 Clipper::interpolateNormal(const glm::vec3& startNormal, const glm::vec3& endNormal,
                                     float fromFirstToSecond) {
    return glm::normalize(startNormal * fromFirstToSecond + endNormal * (1 - fromFirstToSecond));
}
} // namespace cg
