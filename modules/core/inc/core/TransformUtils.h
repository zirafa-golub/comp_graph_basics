#pragma once

#include "common/Math.h"
#include "core/BasicTypes.h"
#include "core/Rotation.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

#include <cassert>

namespace cg {
struct TransformUtils {
    static glm::mat4 translateMatrix(const glm::vec3& translate) {
        return glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0),
                         glm::vec4(translate, 1.0f));
    }

    static void addTranslateLeft(glm::mat4& m, glm::vec3 translation) { m[3] += glm::vec4(translation, 0.0f); }

    static glm::mat4 scaleMatrix(const glm::vec3& scale) {
        glm::mat4 result = glm::identity<glm::mat4>();
        result[0][0] = scale.x;
        result[1][1] = scale.y;
        result[2][2] = scale.z;

        return result;
    }

    static void addScaleLeft(glm::mat4& m, glm::vec3 scale) {
        glm::vec4 scale0(scale, 0.0f);
        m[0] *= scale0;
        m[1] *= scale0;
        m[2] *= scale0;
        m[3] *= glm::vec4(scale, 1.0f);
    }

    static glm::mat4 xyzRotationMatrix(const Rotation& rotation) {
        glm::mat4 result = glm::identity<glm::mat4>();

        auto sinX = std::sin(rotation.x.asRad());
        auto cosX = std::cos(rotation.x.asRad());
        auto sinY = std::sin(rotation.y.asRad());
        auto cosY = std::cos(rotation.y.asRad());
        auto sinZ = std::sin(rotation.z.asRad());
        auto cosZ = std::cos(rotation.z.asRad());
        float sinXcosZ = sinX * cosZ;
        float cosXsinZ = cosX * sinZ;
        float cosXcosZ = cosX * cosZ;
        float sinXsinZ = sinX * sinZ;

        result[0][0] = cosY * cosZ;
        result[0][1] = sinY * sinZ;
        result[0][2] = -sinY;
        result[1][0] = sinXcosZ * sinY - cosXsinZ;
        result[1][1] = sinXsinZ * sinY + cosXcosZ;
        result[1][2] = sinX * cosY;
        result[2][0] = cosXcosZ * sinY - sinXsinZ;
        result[2][1] = cosXsinZ * sinY - sinXcosZ;
        result[2][2] = cosX * cosY;
        result[3][3] = 1;

        return result;
    }

    static void addRotationLeftToScaleMatrix(glm::mat4& mat, const glm::mat4& rot) {
        assert(mat[3] == glm::vec4(0, 0, 0, 1) && "No translation can be present in the input matrix");

        mat[0] = rot[0] * mat[0][0];
        mat[1] = rot[1] * mat[1][1];
        mat[2] = rot[2] * mat[2][2];
    }

    static glm::mat4 rotationToBasisMatrix(const glm::vec3& u, const glm::vec3& v, const glm::vec3& w) {
        assert(areFloatsEqualTolerance(glm::length(u), 1.0f, 0.000005f));
        assert(areFloatsEqualTolerance(glm::length(v), 1.0f, 0.000005f));
        assert(areFloatsEqualTolerance(glm::length(w), 1.0f, 0.000005f));
        assert(areFloatsEqualTolerance(glm::dot(u, v), 0.000005f));
        assert(areFloatsEqualTolerance(glm::dot(u, w), 0.000005f));
        assert(areFloatsEqualTolerance(glm::dot(v, w), 0.000005f));

        return glm::mat4({u.x, v.x, w.x, 0}, {u.y, v.y, w.y, 0}, {u.z, v.z, w.z, 0}, {0, 0, 0, 1});
    }

    static glm::mat4 viewToCanonicalMatrix(Size2d viewPlaneSize, float near, float far) {
        // NOTE: Locally, the camera always looks down the -z axis direction
        float right = viewPlaneSize.width / 2;
        float left = -right;
        float top = viewPlaneSize.height / 2;
        float bottom = -top;

        return glm::mat4({2 / viewPlaneSize.width, 0, 0, 0}, {0, 2 / viewPlaneSize.height, 0, 0},
                         {0, 0, 2 / (near - far), 0},
                         {-(right + left) / viewPlaneSize.width, -(top + bottom) / viewPlaneSize.height,
                          -(near + far) / (near - far), 1});
    }
};
} // namespace cg
