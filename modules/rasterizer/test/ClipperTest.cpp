#include "core/MeshData.h"
#include "core/PerspectiveCamera.h"
#include "rasterizer/Clipper.h"

#include "gtest/gtest.h"

#include <vector>

using namespace cg;
using namespace cg::angle_literals;

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
protected:
    ClipperTest() : clipper_(frustum_) {}

    Clipper clipper_;

public:
    bool meshContainsVertex(const MeshData& mesh, const Point& vertex) {
        for (const auto& v : mesh.vertices()) {
            if (v == vertex) {
                return true;
            }
        }
        return false;
    }

    bool areTrianglesValid(const MeshData& mesh) {
        for (const auto& triangle : mesh.triangles()) {
            for (const auto& otherTriangle : mesh.triangles()) {
                if (triangle == otherTriangle && &triangle != &otherTriangle) {
                    return false;
                }
            }
            if (triangle[0] == triangle[1] || triangle[0] == triangle[2] || triangle[1] == triangle[2]) {
                return false;
            }
            for (auto vi : triangle) {
                if (vi > mesh.vertices().size()) {
                    return false;
                }
            }
        }
        return true;
    }

    bool meshContainsVertexNear(const MeshData& mesh, const Point& nearVertex, float tolerance = 0.000001f) {
        for (const auto& v : mesh.vertices()) {
            if (glm::length(nearVertex - v) <= tolerance) {
                return true;
            }
        }
        return false;
    }
};

TEST_F(ClipperTest, clip_meshInsideFrustum_shouldNotClip) {
    std::vector<Point> vertices = {{1, 1, 4}, {2, 3, 5}, {4, -3, 6}};
    std::vector<MeshData::TriangleIndices> triangles = {{0, 1, 2}};
    MeshData meshData(std::move(vertices), std::move(triangles));

    MeshData clippedMeshData = clipper_.clip(meshData);

    EXPECT_EQ(clippedMeshData.vertices(), meshData.vertices());
    EXPECT_EQ(clippedMeshData.triangles(), meshData.triangles());
}

TEST_F(ClipperTest, clip_meshOutsideFrustum_shouldClipAll) {
    std::vector<Point> vertices = {{1, 1, 11}, {2, 3, 12}, {4, -3, 13}};
    std::vector<MeshData::TriangleIndices> triangles = {{0, 1, 2}};
    MeshData meshData(std::move(vertices), std::move(triangles));

    MeshData clippedMeshData = clipper_.clip(meshData);

    EXPECT_TRUE(clippedMeshData.vertices().empty());
    EXPECT_TRUE(clippedMeshData.triangles().empty());
}

TEST_F(ClipperTest, clip_oneVertexOuside_shouldSplitIntoTwoTriangles) {
    Point outsideVertex(6, 2, 12);
    std::vector<Point> insideVertices = {{6, -2, 8}, {6, 2, 8}};
    std::vector<Point> vertices = insideVertices;
    vertices.push_back(outsideVertex);
    std::vector<MeshData::TriangleIndices> triangles = {{0, 1, 2}};
    MeshData meshData(std::move(vertices), std::move(triangles));

    MeshData clippedMeshData = clipper_.clip(meshData);

    EXPECT_EQ(clippedMeshData.vertices().size(), 4);
    EXPECT_TRUE(meshContainsVertex(clippedMeshData, insideVertices[0]));
    EXPECT_TRUE(meshContainsVertex(clippedMeshData, insideVertices[1]));
    EXPECT_TRUE(meshContainsVertexNear(clippedMeshData, Point(6, 0, 10)));
    EXPECT_TRUE(areTrianglesValid(clippedMeshData));
    EXPECT_EQ(clippedMeshData.triangles().size(), 2);
}

TEST_F(ClipperTest, clip_twoVerticesOuside_shouldClipTriangle) {
    std::vector<Point> outsideVertices = {{4, 2, 12}, {4, -2, 12}};
    Point insideVertex(4, 2, 8);
    std::vector<Point> vertices;
    vertices.push_back(insideVertex);
    vertices.append_range(outsideVertices);
    std::vector<MeshData::TriangleIndices> triangles = {{0, 1, 2}};
    MeshData meshData(std::move(vertices), std::move(triangles));

    MeshData clippedMeshData = clipper_.clip(meshData);

    EXPECT_EQ(clippedMeshData.vertices().size(), 3);
    EXPECT_TRUE(meshContainsVertex(clippedMeshData, meshData.vertices()[0]));
    EXPECT_TRUE(meshContainsVertexNear(clippedMeshData, Point(4, 2, 10)));
    EXPECT_TRUE(meshContainsVertexNear(clippedMeshData, Point(4, 0, 10)));
    EXPECT_TRUE(areTrianglesValid(clippedMeshData));
    EXPECT_EQ(clippedMeshData.triangles().size(), 1);
}

TEST_F(ClipperTest, clip_multipleTrianglesInside_shouldNotDuplicateExistingVertices) {
    std::vector<Point> vertices = {{1, 1, 4}, {2, 3, 5}, {4, -3, 6}, {6, 5, 8}};
    std::vector<MeshData::TriangleIndices> triangles = {{0, 1, 2}, {{0, 1, 3}}};
    MeshData meshData(std::move(vertices), std::move(triangles));

    MeshData clippedMeshData = clipper_.clip(meshData);

    EXPECT_EQ(clippedMeshData.vertices(), meshData.vertices());
    EXPECT_EQ(clippedMeshData.triangles(), meshData.triangles());
}

TEST_F(ClipperTest, clip_vertexAboveTwoPlanes_shouldClipAgainstAppropriatePlane) {
    std::vector<Point> outsideVertices = {{1, 0, -5}, {1, -10, 5}};
    Point insideVertex(1, 0, 5);
    std::vector<Point> vertices;
    vertices.push_back(insideVertex);
    vertices.append_range(outsideVertices);
    std::vector<MeshData::TriangleIndices> triangles = {{0, 1, 2}};
    MeshData meshData(std::move(vertices), std::move(triangles));

    MeshData clippedMeshData = clipper_.clip(meshData);

    EXPECT_EQ(clippedMeshData.vertices().size(), 3);
    EXPECT_TRUE(meshContainsVertex(clippedMeshData, insideVertex));
    EXPECT_TRUE(meshContainsVertexNear(clippedMeshData, Point(1, 0, 1)));
    EXPECT_TRUE(meshContainsVertexNear(clippedMeshData, Point(1, -5 * std::sqrt(3) + camera_.position().y, 5)));
    EXPECT_TRUE(areTrianglesValid(clippedMeshData));
    EXPECT_EQ(clippedMeshData.triangles().size(), 1);
}
