#include "rasterizer/BackFaceCuller.h"

#include "core/BasicTypes.h"
#include "core/MeshData.h"

#include "glm/vec3.hpp"
#include "gtest/gtest.h"

using namespace cg;

TEST(BackFaceCullerTest, shouldCullFaceNormal_facedAway_shouldReturnTrue) {
    glm::vec3 faceNormal = {1, 1, 1};
    Point vertex = {3, 1, 5};
    Point cameraPosition = {1, 0, 0};
    BackFaceCuller bfc(cameraPosition);

    EXPECT_TRUE(bfc.shouldCull(vertex, faceNormal));
}

TEST(BackFaceCullerTest, shouldCullFaceNormal_facedTowards_shouldReturnFalse) {
    glm::vec3 faceNormal = {-1, -1, -1};
    Point vertex = {3, 1, 5};
    Point cameraPosition = {1, 0, 0};
    BackFaceCuller bfc(cameraPosition);

    EXPECT_FALSE(bfc.shouldCull(vertex, faceNormal));
}

TEST(BackFaceCullerTest, shouldCullFaceNormal_rightAngle_shouldReturnTrue) {
    glm::vec3 faceNormal = {-1, 0, 0};
    Point vertex = {1, 1, 5};
    Point cameraPosition = {1, 0, 0};
    BackFaceCuller bfc(cameraPosition);

    EXPECT_TRUE(bfc.shouldCull(vertex, faceNormal));
}

TEST(BackFaceCullerTest, shouldCullTriangle_facedAway_shouldReturnTrue) {
    std::vector<Point> vertices = {{3.0f, -1.0f, 5.0f}, {3.0f, 2.0f, 5.0f}, {-2.0f, -1.0f, 6.0}};
    TriangleIndices triangle = {0, 1, 2};
    glm::vec3 cameraPosition = {1, 0, 0};
    BackFaceCuller bfc(cameraPosition);

    EXPECT_TRUE(bfc.shouldCull(vertices, triangle));
}

TEST(BackFaceCullerTest, shouldCullTriangle_facedTowards_shouldReturnFalse) {
    std::vector<Point> vertices = {{3.0f, -1.0f, 5.0f}, {-2.0f, -1.0f, 6.0}, {3.0f, 2.0f, 5.0f}};
    TriangleIndices triangle = {0, 1, 2};
    glm::vec3 cameraPosition = {1, 0, 0};
    BackFaceCuller bfc(cameraPosition);

    EXPECT_FALSE(bfc.shouldCull(vertices, triangle));
}

TEST(BackFaceCullerTest, shouldCullTriangle_rightAngle_shouldReturnTrue) {
    std::vector<Point> vertices = {{1.0f, -1.0f, 5.0f}, {1.0f, -1.0f, 6.0}, {1.0f, 2.0f, 5.0f}};
    TriangleIndices triangle = {0, 1, 2};
    glm::vec3 cameraPosition = {1, 0, 0};
    BackFaceCuller bfc(cameraPosition);

    EXPECT_TRUE(bfc.shouldCull(vertices, triangle));
}
