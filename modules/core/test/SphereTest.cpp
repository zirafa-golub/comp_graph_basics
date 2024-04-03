#include "core/Sphere.h"
#include "TestUtils.h"

#include "glm/geometric.hpp"
#include "gtest/gtest.h"

#include <iomanip>
#include <iostream>
#include <limits>

using namespace cg;

TEST(SphereTest, center_shouldReturnPosition) {
    Sphere sphere{3};
    Point p{1, 2, 3};
    sphere.setPosition(p);

    EXPECT_EQ(sphere.center(), p);
}

TEST(SphereTest, hit_twoHits_shouldReturnExpected) {
    Sphere sphere{3};
    sphere.setPosition(5, 0, 0);
    sphere.update();

    Ray ray{{0, 0, 0}, {1, 0, 0}};

    auto result = sphere.hit(ray, 1, 6);

    ASSERT_TRUE(result.has_value());
    auto& hit = result.value();
    EXPECT_EQ(hit.hitShape, &sphere);
    EXPECT_EQ(hit.ray, ray);
    EXPECT_FLOAT_EQ(hit.tHit, 2);
    assertVec3FloatEqual(hit.unitNormal, glm::vec3(-1, 0, 0));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}

TEST(SphereTest, hit_twoHitsOneOutOfRange_shouldReturnExpected) {
    Sphere sphere{4};
    sphere.setPosition(0, 0, 0);
    sphere.update();

    Ray ray{{0, 0, 0}, {2, 0, 0}};

    auto result = sphere.hit(ray, 1, 6);

    ASSERT_TRUE(result.has_value());
    auto& hit = result.value();
    EXPECT_EQ(hit.hitShape, &sphere);
    EXPECT_EQ(hit.ray, ray);
    EXPECT_EQ(hit.tHit, 2);
    assertVec3FloatEqual(hit.unitNormal, glm::vec3(-1, 0, 0));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}

TEST(SphereTest, hit_oneHit_shouldReturnExpected) {
    Sphere sphere{3};
    sphere.setPosition(4, 0, 0);
    sphere.update();

    Ray ray{{0, 3, 0}, {1, 0, 0}};

    auto result = sphere.hit(ray, 1, 6);

    ASSERT_TRUE(result.has_value());
    auto& hit = result.value();
    EXPECT_EQ(hit.hitShape, &sphere);
    EXPECT_EQ(hit.ray, ray);
    EXPECT_EQ(hit.tHit, 4);
    assertVec3FloatEqual(hit.unitNormal, glm::vec3(0, 1, 0));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}

TEST(SphereTest, hit_oneHitOutOfRange_shouldReturnError) {
    Sphere sphere{3};
    sphere.setPosition(4, 0, 0);
    sphere.update();

    Ray ray{{0, 3, 0}, {1, 0, 0}};

    auto result = sphere.hit(ray, 1, 3);

    ASSERT_FALSE(result.has_value());
    auto& error = result.error();
    EXPECT_EQ(error, ErrorCode::NotFound);
}

TEST(SphereTest, hit_miss_shouldReturnError) {
    Sphere sphere{3};
    sphere.setPosition(4, 0, 0);
    sphere.update();

    Ray ray{{0, 0, 0}, {0, 1, 0}};

    auto result = sphere.hit(ray, 0, std::numeric_limits<float>::infinity());

    ASSERT_FALSE(result.has_value());
    auto& error = result.error();
    EXPECT_EQ(error, ErrorCode::NotFound);
}

TEST(SphereTest, generateMesh_validInput_shouldGenerateCorrectMesh) {
    Sphere sphere{2};
    sphere.generateMesh(4, 6);

    std::vector<Point> expectedVertices = {{0, 2.00000000, 0},
                                           {1.41421354, 1.41421354, 0},
                                           {0.70710671, 1.41421354, -1.22474492},
                                           {-0.70710683, 1.41421354, -1.22474480},
                                           {-1.41421354, 1.41421354, 0},
                                           {-0.70710665, 1.41421354, 1.22474492},
                                           {0.70710665, 1.41421354, 1.22474492},
                                           {2, 0, 0},
                                           {1, 0, -1.73205090},
                                           {-1, 0, -1.73205078},
                                           {-2, 0, 0},
                                           {-1, 0, 1.73205090},
                                           {1, 0, 1.73205090},
                                           {1.41421354, -1.41421354, 0},
                                           {0.70710671, -1.41421354, -1.22474492},
                                           {-0.70710683, -1.41421354, -1.22474480},
                                           {-1.41421354, -1.41421354, 0.00000012},
                                           {-0.70710665, -1.41421354, 1.22474492},
                                           {0.70710665, -1.41421354, 1.22474492},
                                           {0, -2, 0}};
    std::vector<MeshData::TriangleIndices> expectedTriangles = {
        {0, 1, 0},    {1, 2, 0},    {2, 3, 0},    {3, 4, 0},    {4, 5, 0},    {5, 0, 0},   {7, 8, 1},    {8, 2, 1},
        {8, 9, 2},    {9, 3, 2},    {9, 10, 3},   {10, 4, 3},   {10, 11, 4},  {11, 5, 4},  {11, 12, 5},  {12, 6, 5},
        {12, 7, 6},   {7, 1, 6},    {13, 14, 7},  {14, 8, 7},   {14, 15, 8},  {15, 9, 8},  {15, 16, 9},  {16, 10, 9},
        {16, 17, 10}, {17, 11, 10}, {17, 18, 11}, {18, 12, 11}, {18, 13, 12}, {13, 7, 12}, {13, 19, 14}, {14, 19, 15},
        {15, 19, 16}, {16, 19, 17}, {17, 19, 18}, {18, 19, 13}};

    const MeshData& mesh = sphere.meshData();
    EXPECT_EQ(mesh.vertices().size(), 20);
    EXPECT_EQ(mesh.triangles().size(), 36);

    for (const Point& point : mesh.vertices()) {
        EXPECT_FLOAT_EQ(glm::length(point), sphere.radius());
    }
    for (const MeshData::TriangleIndices& triangle : mesh.triangles()) {
        for (unsigned vertexIndex : triangle) {
            EXPECT_GE(vertexIndex, 0);
            EXPECT_LT(vertexIndex, mesh.vertices().size());
        }
    }

    for (size_t i = 0; i < mesh.vertices().size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(mesh.vertices()[i][j], expectedVertices[i][j], 0.000001f);
        }
    }
    for (size_t i = 0; i < mesh.triangles().size(); ++i) {
        EXPECT_EQ(mesh.triangles()[i], expectedTriangles[i]);
    }
}

#ifndef NDEBUG

TEST(SphereTest, generateMesh_badVerticalSegCount_shouldTriggerAssert) {
    Sphere sphere{2};
    ASSERT_DEATH(sphere.generateMesh(1, 10), ".*");
}

TEST(SphereTest, generateMesh_badHorizontalSegCount_shouldTriggerAssert) {
    Sphere sphere{2};
    ASSERT_DEATH(sphere.generateMesh(10, 2), ".*");
}

TEST(SphereTest, meshData_generateMeshNotCalled_shouldTriggerAssert) {
    Sphere sphere{2};
    ASSERT_DEATH(sphere.meshData(), ".*");
}

#endif
