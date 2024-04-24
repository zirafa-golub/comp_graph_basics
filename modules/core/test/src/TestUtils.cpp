#include "core/Color.h"
#include "core/TestUtils.h"

#include "glm/geometric.hpp"
#include "gtest/gtest.h"

using namespace cg;

void assertColorsFloatEqual(const Color& left, const Color& right) {
    EXPECT_FLOAT_EQ(left.r(), right.r());
    EXPECT_FLOAT_EQ(left.g(), right.g());
    EXPECT_FLOAT_EQ(left.b(), right.b());
}

void assertSize2dFloatEqual(const Size2d& left, const Size2d& right) {
    EXPECT_FLOAT_EQ(left.width, right.width);
    EXPECT_FLOAT_EQ(left.height, right.height);
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
