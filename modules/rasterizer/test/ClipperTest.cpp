#include "common/Math.h"
#include "core/MeshData.h"
#include "core/PerspectiveCamera.h"
#include "rasterizer/Clipper.h"
#include "test_utils/Utils.h"

#include "gtest/gtest.h"

#include <vector>

using namespace cg;
using namespace cg::angle_literals;
using namespace ::testing;

class FrustumIntersectTest : public testing::Test {
protected:
    FrustumIntersectTest() : camera_(initCamera()), frustum_(camera_.frustumPoints()) {}

    PerspectiveCamera camera_;
    FrustumIntersect frustum_;

private:
    PerspectiveCamera initCamera() {
        PerspectiveCamera camera;
        camera.setPosition(Point(0.5f, -0.5f, 0));
        camera.setViewDirection(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
        camera.setViewPlaneDistance(1.0f);
        camera.setViewLimit(10.0f);
        camera.setAspectRatio(1.0f);
        camera.setFieldOfView(120_deg);
        camera.update();
        return camera;
    }
};

TEST_F(FrustumIntersectTest, planesBelowVertex_onePlaneBelow_shouldReturnExpected) {
    Point aboveTop(1, 9, 5);
    Point aboveBottom(-2, -12, 5);
    Point aboveNear(0, 0, 0.5);
    Point aboveFar(4, 8, 11);
    Point aboveLeft(10, -1, 4);
    Point aboveRight(-14, 2, 6);

    EXPECT_EQ(frustum_.planesBelowVertex(aboveTop, FrustumIntersect::AllPlanes), FrustumIntersect::Plane::Top);
    EXPECT_EQ(frustum_.planesBelowVertex(aboveBottom, FrustumIntersect::AllPlanes), FrustumIntersect::Plane::Bottom);
    EXPECT_EQ(frustum_.planesBelowVertex(aboveNear, FrustumIntersect::AllPlanes), FrustumIntersect::Plane::Near);
    EXPECT_EQ(frustum_.planesBelowVertex(aboveFar, FrustumIntersect::AllPlanes), FrustumIntersect::Plane::Far);
    EXPECT_EQ(frustum_.planesBelowVertex(aboveLeft, FrustumIntersect::AllPlanes), FrustumIntersect::Plane::Left);
    EXPECT_EQ(frustum_.planesBelowVertex(aboveRight, FrustumIntersect::AllPlanes), FrustumIntersect::Plane::Right);
}

TEST_F(FrustumIntersectTest, planesBelowVertex_insideFrustum_shouldReturnNoPlanes) {
    Point insideFrustum(2, 5, 5);

    EXPECT_EQ(frustum_.planesBelowVertex(insideFrustum, FrustumIntersect::AllPlanes), FrustumIntersect::NoPlanes);
}

TEST_F(FrustumIntersectTest, planesBelowVertex_multiplePlanesBelow_shouldReturnExpected) {
    Point aboveTopLeft(11, 10, 5);
    Point aboveFarRightBottom(-20, -21, 11);

    EXPECT_EQ(frustum_.planesBelowVertex(aboveTopLeft, FrustumIntersect::AllPlanes),
              FrustumIntersect::Plane::Top | FrustumIntersect::Plane::Left);
    EXPECT_EQ(frustum_.planesBelowVertex(aboveFarRightBottom, FrustumIntersect::AllPlanes),
              FrustumIntersect::Plane::Bottom | FrustumIntersect::Plane::Right | FrustumIntersect::Plane::Far);
}

TEST_F(FrustumIntersectTest, planesBelowVertex_planesSubsetTested_shouldReturnExpected) {
    Point aboveNearRightBottom(-4, -3, -1);

    EXPECT_EQ(frustum_.planesBelowVertex(aboveNearRightBottom, FrustumIntersect::AllPlanes),
              FrustumIntersect::Plane::Bottom | FrustumIntersect::Plane::Right | FrustumIntersect::Plane::Near);
    EXPECT_EQ(
        frustum_.planesBelowVertex(aboveNearRightBottom, FrustumIntersect::Plane::Near | FrustumIntersect::Plane::Left),
        FrustumIntersect::Near);
}

class ClipperTest : public FrustumIntersectTest {
public:
    AssertionResult meshContainsVertex(const MeshData& mesh, const Point& vertex) {
        for (const auto& v : mesh.vertices()) {
            if (v == vertex) {
                return AssertionSuccess();
            }
        }
        return AssertionFailure() << "Requested vertex: " << vertex << " not found in mesh.";
    }

    AssertionResult areTrianglesValid(const MeshData& mesh) {
        for (const auto& triangle : mesh.triangles()) {
            for (const auto& otherTriangle : mesh.triangles()) {
                if (triangle == otherTriangle && &triangle != &otherTriangle) {
                    return AssertionFailure() << "Duplicate triangles detected in mesh.";
                }
            }

            if (triangle[0].vertex == triangle[1].vertex || triangle[0].vertex == triangle[2].vertex ||
                triangle[1].vertex == triangle[2].vertex) {
                return AssertionFailure() << "Duplicate vertices detected in a triangle.";
            }

            for (const auto& vd : triangle) {
                if (vd.vertex >= mesh.vertices().size()) {
                    return AssertionFailure() << "Invalid vertex index detected: " << vd.vertex;
                }
                if (vd.vertexNormal >= mesh.vertexNormals().size()) {
                    return AssertionFailure() << "Invalid vertex normal index detected: " << vd.vertexNormal;
                }
                if (!areFloatsEqualTolerance(glm::length(mesh.vertexNormals()[vd.vertexNormal]), 1.0f, tolerance)) {
                    return AssertionFailure()
                           << "Vertex normal with invalid length detected: " << mesh.vertexNormals()[vd.vertexNormal];
                }
            }
        }
        return AssertionSuccess();
    }

    AssertionResult meshContainsVertexNear(const MeshData& mesh, const Point& vertex) {
        if (findVertexIndexOfVertexNear(mesh, vertex) != MeshData::invalidIndex) {
            return AssertionSuccess();
        } else {
            return AssertionFailure() << "No vertex found in the vicinity of: " << vertex;
        }
    }

    AssertionResult isVertexNormalNearEqualTo(const MeshData& mesh, const Point& vertex,
                                              const glm::vec3& expectedNormal) {
        int vertexIndex = findVertexIndexOfVertexNear(mesh, vertex);
        if (vertexIndex == MeshData::invalidIndex) {
            return AssertionFailure() << "Provided vertex not found in mesh: " << vertex;
        }
        for (const auto& triangleData : mesh.triangles()) {
            for (const auto& vertexData : triangleData) {
                if (vertexData.vertex == vertexIndex) {
                    const auto& normal = mesh.vertexNormals()[vertexData.vertexNormal];
                    if (glm::length(normal - expectedNormal) <= tolerance) {
                        return AssertionSuccess();
                    } else {
                        return AssertionFailure() << "Wrong value for vertex normal. Value in mesh: " << normal
                                                  << ", expected: " << expectedNormal;
                    }
                }
            }
        }
        return AssertionFailure() << "Requested vertex not referenced in mesh triangles: " << vertex;
    }

    static constexpr float tolerance = 0.000001f;

protected:
    ClipperTest() : clipper_(frustum_) {}

    Clipper clipper_;

private:
    MeshData::Index findVertexIndexOfVertexNear(const MeshData& mesh, const Point& vertex) {
        for (int i = 0; i < mesh.vertices().size(); ++i) {
            if (glm::length(vertex - mesh.vertices()[i]) <= tolerance) {
                return i;
            }
        }
        return MeshData::invalidIndex;
    }
};

TEST_F(ClipperTest, clip_meshInsideFrustum_shouldNotClip) {
    std::vector<Point> vertices = {{1, 1, 4}, {2, 3, 5}, {4, -3, 6}};
    std::vector<Point> vertexNormals = {{0, 1, 0}};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 1, 2, 0)};
    const MeshData meshData(std::move(vertices), std::move(vertexNormals), std::move(triangles));

    const MeshData clippedMeshData = clipper_.clip(meshData);

    assertSpansEqual(clippedMeshData.vertices(), meshData.vertices());
    assertSpansEqual(clippedMeshData.vertexNormals(), meshData.vertexNormals());
    assertSpansEqual(clippedMeshData.triangles(), meshData.triangles());
}

TEST_F(ClipperTest, clip_meshOutsideFrustum_shouldClipAll) {
    std::vector<Point> vertices = {{1, 1, 11}, {2, 3, 12}, {4, -3, 13}};
    std::vector<Point> vertexNormals = {{0, 1, 0}};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 1, 2, 0)};
    MeshData meshData(std::move(vertices), std::move(vertexNormals), std::move(triangles));

    MeshData clippedMeshData = clipper_.clip(meshData);

    EXPECT_TRUE(clippedMeshData.vertices().empty());
    EXPECT_TRUE(clippedMeshData.vertexNormals().empty());
    EXPECT_TRUE(clippedMeshData.triangles().empty());
}

TEST_F(ClipperTest, clip_oneVertexOuside_shouldSplitIntoTwoTriangles) {
    Point outsideVertex(6, 2, 12);
    std::vector<Point> insideVertices = {{6, -2, 8}, {6, 2, 8}};
    std::vector<Point> vertices = insideVertices;
    vertices.push_back(outsideVertex);
    std::vector<Point> vertexNormals = {glm::normalize(glm::vec3(1, -1, -1)), glm::normalize(glm::vec3(1, 1, -1)),
                                        glm::normalize(glm::vec3(1, 1, 1))};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 1, 2)};
    MeshData meshData(std::move(vertices), std::move(vertexNormals), std::move(triangles));

    MeshData clippedMeshData = clipper_.clip(meshData);

    Point expectedNewVertex1 = {6, 0, 10};
    Point expectedNormal1 = {1, 0, 0};
    Point expectedNewVertex2 = {6, 2, 10};
    Point expectedNormal2 = glm::normalize(glm::vec3(1, 1, 0));

    EXPECT_EQ(clippedMeshData.vertices().size(), 4);
    EXPECT_TRUE(meshContainsVertex(clippedMeshData, insideVertices[0]));
    EXPECT_TRUE(meshContainsVertex(clippedMeshData, insideVertices[1]));
    EXPECT_TRUE(meshContainsVertexNear(clippedMeshData, expectedNewVertex1));
    EXPECT_TRUE(meshContainsVertexNear(clippedMeshData, expectedNewVertex2));
    EXPECT_TRUE(isVertexNormalNearEqualTo(clippedMeshData, expectedNewVertex1, expectedNormal1));
    EXPECT_TRUE(isVertexNormalNearEqualTo(clippedMeshData, expectedNewVertex2, expectedNormal2));
    EXPECT_TRUE(areTrianglesValid(clippedMeshData));
    EXPECT_EQ(clippedMeshData.triangles().size(), 2);
}

TEST_F(ClipperTest, clip_twoVerticesOuside_shouldClipTriangle) {
    std::vector<Point> outsideVertices = {{4, 2, 12}, {4, -2, 12}};
    Point insideVertex(4, 2, 8);
    std::vector<Point> vertices;
    vertices.push_back(insideVertex);
    vertices.append_range(outsideVertices);
    std::vector<Point> vertexNormals = {glm::normalize(glm::vec3(1, 1, -1)), glm::normalize(glm::vec3(1, 1, 1)),
                                        glm::normalize(glm::vec3(1, -1, 1))};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 1, 2)};
    MeshData meshData(std::move(vertices), std::move(vertexNormals), std::move(triangles));

    MeshData clippedMeshData = clipper_.clip(meshData);

    Point expectedNewVertex1 = {4, 2, 10};
    Point expectedNormal1 = glm::normalize(glm::vec3(1, 1, 0));
    Point expectedNewVertex2 = {4, 0, 10};
    Point expectedNormal2 = glm::vec3(1, 0, 0);

    EXPECT_EQ(clippedMeshData.vertices().size(), 3);
    EXPECT_TRUE(meshContainsVertex(clippedMeshData, meshData.vertices()[0]));
    EXPECT_TRUE(meshContainsVertexNear(clippedMeshData, expectedNewVertex1));
    EXPECT_TRUE(meshContainsVertexNear(clippedMeshData, expectedNewVertex2));
    EXPECT_TRUE(isVertexNormalNearEqualTo(clippedMeshData, expectedNewVertex1, expectedNormal1));
    EXPECT_TRUE(isVertexNormalNearEqualTo(clippedMeshData, expectedNewVertex2, expectedNormal2));
    EXPECT_TRUE(areTrianglesValid(clippedMeshData));
    EXPECT_EQ(clippedMeshData.triangles().size(), 1);
}

TEST_F(ClipperTest, clip_multipleTrianglesInside_shouldNotDuplicateExistingVertices) {
    std::vector<Point> vertices = {{1, 1, 4}, {2, 3, 5}, {4, -3, 6}, {6, 5, 8}};
    std::vector<Point> vertexNormals = {{0, 1, 0}};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 1, 2, 0), MeshData::createTriangle(0, 1, 3, 0)};
    const MeshData meshData(std::move(vertices), std::move(vertexNormals), std::move(triangles));

    const MeshData clippedMeshData = clipper_.clip(meshData);

    assertSpansEqual(clippedMeshData.vertices(), meshData.vertices());
    assertSpansEqual(clippedMeshData.vertexNormals(), meshData.vertexNormals());
    assertSpansEqual(clippedMeshData.triangles(), meshData.triangles());
}

TEST_F(ClipperTest, clip_vertexAboveTwoPlanes_shouldClipAgainstAppropriatePlane) {
    std::vector<Point> outsideVertices = {{1, 0, -5}, {1, -10, 5}};
    Point insideVertex(1, 0, 5);
    std::vector<Point> vertices;
    vertices.push_back(insideVertex);
    vertices.append_range(outsideVertices);
    std::vector<Point> vertexNormals = {{0, 1, 0}};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 1, 2, 0)};
    MeshData meshData(std::move(vertices), std::move(vertexNormals), std::move(triangles));

    MeshData clippedMeshData = clipper_.clip(meshData);

    EXPECT_EQ(clippedMeshData.vertices().size(), 3);
    EXPECT_TRUE(meshContainsVertex(clippedMeshData, insideVertex));
    EXPECT_TRUE(meshContainsVertexNear(clippedMeshData, Point(1, 0, 1)));
    EXPECT_TRUE(meshContainsVertexNear(clippedMeshData, Point(1, -5 * std::sqrt(3) + camera_.position().y, 5)));
    EXPECT_TRUE(areTrianglesValid(clippedMeshData));
    EXPECT_EQ(clippedMeshData.triangles().size(), 1);
}
