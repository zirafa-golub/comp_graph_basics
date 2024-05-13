#include "test_utils/Utils.h"

void assertVec3FloatEqual(const glm::vec3& left, const glm::vec3& right) {
    EXPECT_FLOAT_EQ(left.x, right.x);
    EXPECT_FLOAT_EQ(left.y, right.y);
    EXPECT_FLOAT_EQ(left.z, right.z);
}

void assertVec4FloatEqual(const glm::vec4& left, const glm::vec4& right) {
    EXPECT_FLOAT_EQ(left.x, right.x);
    EXPECT_FLOAT_EQ(left.y, right.y);
    EXPECT_FLOAT_EQ(left.z, right.z);
    EXPECT_FLOAT_EQ(left.w, right.w);
}

void assertMat4FloatEqual(const glm::mat4& left, const glm::mat4& right) {
    for (unsigned i = 0; i < 4; ++i) {
        for (unsigned j = 0; j < 4; ++j) {
            EXPECT_FLOAT_EQ(left[i][j], right[i][j]) << "i: " << i << ", j: " << j;
        }
    }
}

void assertMat4EqualTolerance(const glm::mat4& left, const glm::mat4& right, float tolerance) {
    for (unsigned i = 0; i < 4; ++i) {
        for (unsigned j = 0; j < 4; ++j) {
            EXPECT_NEAR(left[i][j], right[i][j], tolerance) << "i: " << i << ", j: " << j;
        }
    }
}

namespace glm {
std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    os << "x: " << vec.x << ", y: " << vec.y << ", z: " << vec.z;
    return os;
}
} // namespace glm
