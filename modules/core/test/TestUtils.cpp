#include "core/Color.h"
#include "TestUtils.h"

#include "gtest/gtest.h"

using namespace cg;

void assertColorsFloatEqual(const Color& left, const Color& right) {
    EXPECT_FLOAT_EQ(left.r(), right.r());
    EXPECT_FLOAT_EQ(left.g(), right.g());
    EXPECT_FLOAT_EQ(left.b(), right.b());
}

std::expected<HitDesc, Error> UnhittableShape::hit(const Ray& ray, float tMin, float tMax) const {
    return std::unexpected(ErrorCode::NotFound);
}

Color PerfectMirrorMaterial::reflect(const glm::vec3& normal, const glm::vec3& viewerDirection,
                                     const glm::vec3& lightDirection) const {
    return Color(1.0, 1.0, 1.0);
}
