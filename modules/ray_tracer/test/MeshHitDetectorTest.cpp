#include "core/Mesh.h"
#include "hit/MeshHitDetector.h"
#include "test_utils/Utils.h"

#include "glm/geometric.hpp"
#include "gtest/gtest.h"

using namespace cg;

class TestShaderGroup : public ShaderGroup {};

TEST(MeshHitDetectorTest, hitTriangle_hit_shouldReturnExpected) {
    Point vertexA = {0, 0, 3};
    Point vertexB = {4, 0, 3};
    Point vertexC = {0, 4, 3};

    Ray ray({1, 2, 0}, {0, 0, 1});

    auto result = MeshHitDetector::hitTriangle(ray, 1, 10, vertexA, vertexB, vertexC);

    ASSERT_TRUE(result.has_value());
    const MeshHitDetector::TriangleHit& hit = result.value();

    EXPECT_FLOAT_EQ(hit.rayHitVal, 3);
    EXPECT_FLOAT_EQ(hit.beta, 0.25f);
    EXPECT_FLOAT_EQ(hit.gamma, 0.5f);
}

TEST(MeshHitDetectorTest, hitTriangle_edgeHit_shouldReturnNotFound) {
    Point vertexA = {0, 0, 3};
    Point vertexB = {4, 0, 3};
    Point vertexC = {0, 4, 3};

    Ray ray({2, 2, 0}, {0, 0, 2});

    auto result = MeshHitDetector::hitTriangle(ray, 1, 10, vertexA, vertexB, vertexC);

    ASSERT_TRUE(result.has_value());
    const MeshHitDetector::TriangleHit& hit = result.value();

    EXPECT_FLOAT_EQ(hit.rayHitVal, 1.5);
    EXPECT_FLOAT_EQ(hit.beta, 0.5f);
    EXPECT_FLOAT_EQ(hit.gamma, 0.5f);
}

TEST(MeshHitDetectorTest, hitTriangle_miss_shouldReturnNotFound) {
    Point vertexA = {0, 0, 3};
    Point vertexB = {4, 0, 3};
    Point vertexC = {0, 4, 3};

    Ray ray({3, 3, 0}, {0, 0, 1});

    auto result = MeshHitDetector::hitTriangle(ray, 1, 10, vertexA, vertexB, vertexC);

    ASSERT_FALSE(result.has_value());
}

TEST(MeshHitDetectorTest, hitTriangle_missParallel_shouldReturnNotFound) {
    Point vertexA = {0, 0, 3};
    Point vertexB = {4, 0, 3};
    Point vertexC = {0, 4, 3};

    Ray ray({1, 2, 0}, {0, 1, 0});

    auto result = MeshHitDetector::hitTriangle(ray, 1, 10, vertexA, vertexB, vertexC);

    ASSERT_FALSE(result.has_value());
}

TEST(MeshHitDetectorTest, hitTriangle_hitOutOfBounds_shouldReturnNotFound) {
    Point vertexA = {0, 0, 3};
    Point vertexB = {4, 0, 3};
    Point vertexC = {0, 4, 3};

    Ray ray({1, 2, 0}, {0, 0, 1});

    auto result = MeshHitDetector::hitTriangle(ray, 1, 2, vertexA, vertexB, vertexC);

    ASSERT_FALSE(result.has_value());
}

TEST(MeshHitDetectorTest, hitTriangle_hitOnRayBound_shouldReturnExpected) {
    Point vertexA = {0, 0, 3};
    Point vertexB = {4, 0, 3};
    Point vertexC = {0, 4, 3};

    Ray ray({1, 2, 0}, {0, 0, 1});

    auto result = MeshHitDetector::hitTriangle(ray, 1, 3, vertexA, vertexB, vertexC);

    ASSERT_TRUE(result.has_value());
    const MeshHitDetector::TriangleHit& hit = result.value();

    EXPECT_FLOAT_EQ(hit.rayHitVal, 3);
    EXPECT_FLOAT_EQ(hit.beta, 0.25f);
    EXPECT_FLOAT_EQ(hit.gamma, 0.5f);
}

TEST(MeshHitDetectorTest, hit_hit_shouldReturnExpected) {
    Point vertexA = {0, 0, 3};
    Point vertexB = {4, 0, 3};
    Point vertexC = {0, 4, 3};

    std::vector<Point> vertices = {vertexA, vertexB, vertexC};
    std::vector<glm::vec3> normals = {{-1, -1, -1}, {-1, 1, -1}, {1, -1, -1}};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 2, 1)};

    Mesh mesh(MeshData(std::move(vertices), std::move(normals), std::move(triangles)));
    TestShaderGroup shaderGroup;
    shaderGroup.setShape(mesh);
    MeshHitDetector hitDetector;
    hitDetector.setShaderGroup(shaderGroup);

    Ray ray({1, 1, 0}, {0, 0, 1});

    hitDetector.initForFrame();
    auto result = hitDetector.hit(ray, 1, 10);

    ASSERT_TRUE(result.has_value());
    const HitDesc& hit = result.value();

    EXPECT_FLOAT_EQ(hit.rayHitVal, 3);
    EXPECT_EQ(hit.hitShape, &mesh);
    EXPECT_EQ(hit.ray, ray);
    assertVec3FloatEqual(hit.unitNormal, glm::normalize(glm::vec3(-0.5f, -0.5f, -1)));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}

TEST(MeshHitDetectorTest, hit_edgeHit_shouldReturnNotFound) {
    Point vertexA = {0, 0, 3};
    Point vertexB = {4, 0, 3};
    Point vertexC = {0, 4, 3};

    std::vector<Point> vertices = {vertexA, vertexB, vertexC};
    std::vector<glm::vec3> normals = {{-1, -1, -1}, {-1, 1, -1}, {1, -1, -1}};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 2, 1)};

    Mesh mesh(MeshData(std::move(vertices), std::move(normals), std::move(triangles)));
    TestShaderGroup shaderGroup;
    shaderGroup.setShape(mesh);
    MeshHitDetector hitDetector;
    hitDetector.setShaderGroup(shaderGroup);

    Ray ray({2, 2, 0}, {0, 0, 2});

    hitDetector.initForFrame();
    auto result = hitDetector.hit(ray, 1, 10);

    ASSERT_TRUE(result.has_value());
    const HitDesc& hit = result.value();

    EXPECT_FLOAT_EQ(hit.rayHitVal, 1.5);
    EXPECT_EQ(hit.hitShape, &mesh);
    EXPECT_EQ(hit.ray, ray);
    assertVec3FloatEqual(hit.unitNormal, glm::vec3(0, 0, -1));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}

TEST(MeshHitDetectorTest, hit_miss_shouldReturnNotFound) {
    Point vertexA = {0, 0, 3};
    Point vertexB = {4, 0, 3};
    Point vertexC = {0, 4, 3};

    std::vector<Point> vertices = {vertexA, vertexB, vertexC};
    std::vector<glm::vec3> normals = {{-1, -1, -1}, {-1, 1, -1}, {1, -1, -1}};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 2, 1)};

    Mesh mesh(MeshData(std::move(vertices), std::move(normals), std::move(triangles)));
    TestShaderGroup shaderGroup;
    shaderGroup.setShape(mesh);
    MeshHitDetector hitDetector;
    hitDetector.setShaderGroup(shaderGroup);

    Ray ray({3, 3, 0}, {0, 0, 1});

    hitDetector.initForFrame();
    auto result = hitDetector.hit(ray, 1, 10);

    ASSERT_FALSE(result.has_value());
}

TEST(MeshHitDetectorTest, hit_missParallel_shouldReturnNotFound) {
    Point vertexA = {0, 0, 3};
    Point vertexB = {4, 0, 3};
    Point vertexC = {0, 4, 3};

    std::vector<Point> vertices = {vertexA, vertexB, vertexC};
    std::vector<glm::vec3> normals = {{-1, -1, -1}, {-1, 1, -1}, {1, -1, -1}};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 2, 1)};

    Mesh mesh(MeshData(std::move(vertices), std::move(normals), std::move(triangles)));
    TestShaderGroup shaderGroup;
    shaderGroup.setShape(mesh);
    MeshHitDetector hitDetector;
    hitDetector.setShaderGroup(shaderGroup);

    Ray ray({1, 2, 0}, {0, 1, 0});

    hitDetector.initForFrame();
    auto result = hitDetector.hit(ray, 1, 10);

    ASSERT_FALSE(result.has_value());
}

TEST(MeshHitDetectorTest, hit_hitOutOfBounds_shouldReturnNotFound) {
    Point vertexA = {0, 0, 3};
    Point vertexB = {4, 0, 3};
    Point vertexC = {0, 4, 3};

    std::vector<Point> vertices = {vertexA, vertexB, vertexC};
    std::vector<glm::vec3> normals = {{-1, -1, -1}, {-1, 1, -1}, {1, -1, -1}};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 2, 1)};

    Mesh mesh(MeshData(std::move(vertices), std::move(normals), std::move(triangles)));
    TestShaderGroup shaderGroup;
    shaderGroup.setShape(mesh);
    MeshHitDetector hitDetector;
    hitDetector.setShaderGroup(shaderGroup);

    Ray ray({1, 2, 0}, {0, 0, 1});

    hitDetector.initForFrame();
    auto result = hitDetector.hit(ray, 1, 2);

    ASSERT_FALSE(result.has_value());
}

TEST(MeshHitDetectorTest, hit_hitOnRayBound_shouldReturnExpected) {
    Point vertexA = {0, 0, 3};
    Point vertexB = {4, 0, 3};
    Point vertexC = {0, 4, 3};

    std::vector<Point> vertices = {vertexA, vertexB, vertexC};
    std::vector<glm::vec3> normals = {{-1, -1, -1}, {-1, 1, -1}, {1, -1, -1}};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 2, 1)};

    Mesh mesh(MeshData(std::move(vertices), std::move(normals), std::move(triangles)));
    TestShaderGroup shaderGroup;
    shaderGroup.setShape(mesh);
    MeshHitDetector hitDetector;
    hitDetector.setShaderGroup(shaderGroup);

    Ray ray({1, 1, 0}, {0, 0, 1});

    hitDetector.initForFrame();
    auto result = hitDetector.hit(ray, 1, 3);

    ASSERT_TRUE(result.has_value());
    const HitDesc& hit = result.value();

    EXPECT_FLOAT_EQ(hit.rayHitVal, 3);
    EXPECT_EQ(hit.hitShape, &mesh);
    EXPECT_EQ(hit.ray, ray);
    assertVec3FloatEqual(hit.unitNormal, glm::normalize(glm::vec3(-0.5f, -0.5f, -1)));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}

TEST(MeshHitDetectorTest, hit_multipleTriangles_shouldReturnClosest) {
    std::vector<Point> vertices = {{0, 0, 3}, {4, 0, 3}, {0, 4, 3}, {4, 4, 3},
                                   {0, 0, 7}, {4, 0, 7}, {0, 4, 7}, {4, 4, 7}};
    std::vector<glm::vec3> normals = {{0, 0, -1}};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 2, 1, 0), MeshData::createTriangle(1, 2, 3, 0),
                                           MeshData::createTriangle(5, 6, 4, 0), MeshData::createTriangle(5, 7, 6, 0)};

    Mesh mesh(MeshData(std::move(vertices), std::move(normals), std::move(triangles)));
    TestShaderGroup shaderGroup;
    shaderGroup.setShape(mesh);
    MeshHitDetector hitDetector;
    hitDetector.setShaderGroup(shaderGroup);

    Ray ray({1, 2, 0}, {0, 0, 1});

    hitDetector.initForFrame();
    auto result = hitDetector.hit(ray, 1, 10);

    ASSERT_TRUE(result.has_value());
    const HitDesc& hit = result.value();

    EXPECT_FLOAT_EQ(hit.rayHitVal, 3);
    EXPECT_EQ(hit.hitShape, &mesh);
    EXPECT_EQ(hit.ray, ray);
    assertVec3FloatEqual(hit.unitNormal, glm::vec3(0, 0, -1));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}

TEST(MeshHitDetectorTest, hit_closestOutOfBounds_shouldReturnFartherHit) {
    std::vector<Point> vertices = {{0, 0, 3}, {4, 0, 3}, {0, 4, 3}, {4, 4, 3},
                                   {0, 0, 7}, {4, 0, 7}, {0, 4, 7}, {4, 4, 7}};
    std::vector<glm::vec3> normals = {{0, 0, -1}};
    std::vector<TriangleData> triangles = {MeshData::createTriangle(0, 2, 1, 0), MeshData::createTriangle(1, 2, 3, 0),
                                           MeshData::createTriangle(5, 6, 4, 0), MeshData::createTriangle(5, 7, 6, 0)};

    Mesh mesh(MeshData(std::move(vertices), std::move(normals), std::move(triangles)));
    TestShaderGroup shaderGroup;
    shaderGroup.setShape(mesh);
    MeshHitDetector hitDetector;
    hitDetector.setShaderGroup(shaderGroup);

    Ray ray({1, 2, 0}, {0, 0, 1});

    hitDetector.initForFrame();
    auto result = hitDetector.hit(ray, 4, 10);

    ASSERT_TRUE(result.has_value());
    const HitDesc& hit = result.value();

    EXPECT_FLOAT_EQ(hit.rayHitVal, 7);
    EXPECT_EQ(hit.hitShape, &mesh);
    EXPECT_EQ(hit.ray, ray);
    assertVec3FloatEqual(hit.unitNormal, glm::vec3(0, 0, -1));
    assertVec3FloatEqual(hit.unitViewDirection, glm::normalize(-ray.direction()));
}
