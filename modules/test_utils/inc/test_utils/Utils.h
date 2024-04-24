#pragma once

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "gtest/gtest.h"

#include <span>

template <typename T, size_t Extent1, size_t Extent2>
void assertSpansEqual(const std::span<T, Extent1>& left, const std::span<T, Extent2>& right) {
    EXPECT_TRUE(std::equal(left.begin(), left.end(), right.begin()));
}

void assertVec3FloatEqual(const glm::vec3& left, const glm::vec3& right);
void assertVec4FloatEqual(const glm::vec4& left, const glm::vec4& right);
void assertMat4FloatEqual(const glm::mat4& left, const glm::mat4& right);
void assertMat4EqualTolerance(const glm::mat4& left, const glm::mat4& right, float tolerance = 1e-7f);
