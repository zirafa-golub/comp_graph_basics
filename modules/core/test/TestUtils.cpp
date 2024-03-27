#include "core/Color.h"
#include "TestUtils.h"

#include "gtest/gtest.h"

#include "glm/geometric.hpp"
#include "glm/vec3.hpp"

using namespace cg;

void assertColorsFloatEqual(const Color& left, const Color& right) {
    EXPECT_FLOAT_EQ(left.r(), right.r());
    EXPECT_FLOAT_EQ(left.g(), right.g());
    EXPECT_FLOAT_EQ(left.b(), right.b());
}

void assertVec3FloatEqual(const glm::vec3& left, const glm::vec3& right) {
    EXPECT_FLOAT_EQ(left.x, right.x);
    EXPECT_FLOAT_EQ(left.y, right.y);
    EXPECT_FLOAT_EQ(left.z, right.z);
}

void areVectorsParallel(const glm::vec3& left, const glm::vec3& right) {
    assertVec3FloatEqual(glm::normalize(left), glm::normalize(right));
}

std::expected<HitDesc, Error> UnhittableShape::hit(const Ray& ray, float tMin, float tMax) const {
    return std::unexpected(ErrorCode::NotFound);
}

const cg::MeshData& UnhittableShape::meshData() const { return meshData_; }

Color PerfectMirrorMaterial::reflect(const glm::vec3& normal, const glm::vec3& viewerDirection,
                                     const glm::vec3& lightDirection) const {
    return Color(1, 1, 1);
}

cg::Color PerfectMirrorMaterial::surfaceReflectance() const { return Color(1, 1, 1); }
