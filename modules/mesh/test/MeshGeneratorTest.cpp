#include "core/BasicTypes.h"
#include "mesh/MeshGenerator.h"
#include "test_utils/Utils.h"

#include "glm/geometric.hpp"
#include "gtest/gtest.h"

#include <vector>

using namespace cg;

TEST(MeshGeneratorTest, generateSphere_validInput_shouldReturnExpected) {
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
    std::vector<std::array<MeshData::Index, 3>> expectedTriangleIndices = {
        {0, 1, 0},    {1, 2, 0},    {2, 3, 0},    {3, 4, 0},    {4, 5, 0},    {5, 0, 0},   {7, 8, 1},    {8, 2, 1},
        {8, 9, 2},    {9, 3, 2},    {9, 10, 3},   {10, 4, 3},   {10, 11, 4},  {11, 5, 4},  {11, 12, 5},  {12, 6, 5},
        {12, 7, 6},   {7, 1, 6},    {13, 14, 7},  {14, 8, 7},   {14, 15, 8},  {15, 9, 8},  {15, 16, 9},  {16, 10, 9},
        {16, 17, 10}, {17, 11, 10}, {17, 18, 11}, {18, 12, 11}, {18, 13, 12}, {13, 7, 12}, {13, 19, 14}, {14, 19, 15},
        {15, 19, 16}, {16, 19, 17}, {17, 19, 18}, {18, 19, 13}};

    constexpr float radius = 2.0f;
    MeshData mesh = MeshGenerator::generateSphere(radius, 4, 6);
    EXPECT_EQ(mesh.vertices().size(), 20);
    EXPECT_EQ(mesh.triangles().size(), 36);

    for (const Point& point : mesh.vertices()) {
        EXPECT_FLOAT_EQ(glm::length(point), radius);
    }
    for (const TriangleData& triangle : mesh.triangles()) {
        for (VertexData vertexData : triangle) {
            EXPECT_GE(vertexData.vertex, 0);
            EXPECT_LT(vertexData.vertex, mesh.vertices().size());
        }
    }

    for (size_t i = 0; i < mesh.vertices().size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(mesh.vertices()[i][j], expectedVertices[i][j], 0.000001f) << "i: " << i << ", j: " << j;
        }
    }
    for (size_t i = 0; i < mesh.vertices().size(); ++i) {
        EXPECT_NEAR(glm::length(mesh.vertexNormals()[i]), 1.0f, 0.000001f) << "i: " << i;
        EXPECT_FLOAT_EQ(glm::dot(mesh.vertexNormals()[i], mesh.vertices()[i]),
                        glm::length(mesh.vertexNormals()[i]) * glm::length(mesh.vertices()[i]))
            << "i: " << i;
    }
    for (size_t i = 0; i < mesh.triangles().size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_EQ(mesh.triangles()[i][j], VertexData(expectedTriangleIndices[i][j], expectedTriangleIndices[i][j]))
                << "i: " << i << ", j: " << j;
        }
    }
}

TEST(MeshGeneratorTest, generateRectangle_validInput_shouldReturnExpected) {
    std::vector<Point> expectedVertices = {{-1.5f, -1, 0}, {-0.5f, -1, 0}, {0.5f, -1, 0}, {1.5f, -1, 0},
                                           {-1.5f, 0, 0},  {-0.5f, 0, 0},  {0.5f, 0, 0},  {1.5f, 0, 0},
                                           {-1.5f, 1, 0},  {-0.5f, 1, 0},  {0.5f, 1, 0},  {1.5f, 1, 0}};
    std::vector<std::array<MeshData::Index, 3>> expectedTriangleIndices = {
        {0, 1, 4}, {1, 5, 4}, {1, 2, 5}, {2, 6, 5},  {2, 3, 6},  {3, 7, 6},
        {4, 5, 8}, {5, 9, 8}, {5, 6, 9}, {6, 10, 9}, {6, 7, 10}, {7, 11, 10}};

    MeshData mesh = MeshGenerator::generateRectangle({3, 2}, 3, 2);
    EXPECT_EQ(mesh.vertices().size(), 12);
    EXPECT_EQ(mesh.triangles().size(), 12);
    EXPECT_EQ(mesh.vertexNormals().size(), 1);

    for (const TriangleData& triangle : mesh.triangles()) {
        for (VertexData vertexData : triangle) {
            EXPECT_GE(vertexData.vertex, 0);
            EXPECT_LT(vertexData.vertex, mesh.vertices().size());
        }
    }

    for (size_t i = 0; i < mesh.vertices().size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(mesh.vertices()[i][j], expectedVertices[i][j], 0.000001f) << "i: " << i << ", j: " << j;
        }
    }
    for (size_t i = 0; i < mesh.triangles().size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_EQ(mesh.triangles()[i][j], VertexData(expectedTriangleIndices[i][j], 0))
                << "i: " << i << ", j: " << j;
        }
    }
    assertVec3FloatEqual(mesh.vertexNormals()[0], glm::vec3(0, 0, 1));
}

#ifndef NDEBUG

TEST(MeshGeneratorTest, generateSphere_badVerticalSegCount_shouldTriggerAssert) {
    ASSERT_DEATH(MeshGenerator::generateSphere(2.0f, 1, 10), ".*");
}

TEST(MeshGeneratorTest, generateSphere_badHorizontalSegCount_shouldTriggerAssert) {
    ASSERT_DEATH(MeshGenerator::generateSphere(2.0f, 10, 2), ".*");
}

TEST(MeshGeneratorTest, generateRectangle_badHorSegCount_shouldTriggerAssert) {
    ASSERT_DEATH(MeshGenerator::generateRectangle({1, 1}, 0, 10), ".*");
}

TEST(MeshGeneratorTest, generateRectangle_badWidth_shouldTriggerAssert) {
    ASSERT_DEATH(MeshGenerator::generateRectangle({-1, 1}, 10, 10), ".*");
}

TEST(MeshGeneratorTest, generateRectangle_badHeight_shouldTriggerAssert) {
    ASSERT_DEATH(MeshGenerator::generateRectangle({1, -1}, 10, 10), ".*");
}

TEST(MeshGeneratorTest, generateRectangle_badVerSegCount_shouldTriggerAssert) {
    ASSERT_DEATH(MeshGenerator::generateRectangle({1, 1}, 10, 0), ".*");
}

#endif
