#include "core/Transformable.h"

#include "glm/ext/matrix_transform.hpp"
#include "gtest/gtest.h"
#include "TestUtils.h"

#include <numbers>

using namespace cg;

TEST(TransformableTest, toGlobal_shouldGenerateExpected) {
    Transformable t;
    glm::vec3 position = glm::vec3(3, -2, 5);
    glm::vec3 scale = glm::vec3(2, 3, 4);
    Rotation rotation =
        Rotation::fromVec3Rad(glm::vec3(std::numbers::pi_v<float> / 4, std::numbers::pi_v<float> / 2, 0));

    t.setRotation(rotation);
    t.setScale(scale);
    t.setPosition(position);
    t.update();

    glm::mat4 expected = glm::translate(glm::identity<glm::mat4>(), position);
    expected = glm::rotate(expected, rotation.z.asRad(), glm::vec3(0, 0, 1));
    expected = glm::rotate(expected, rotation.y.asRad(), glm::vec3(0, 1, 0));
    expected = glm::rotate(expected, rotation.x.asRad(), glm::vec3(1, 0, 0));
    expected = glm::scale(expected, scale);

    assertMatricesEqualTolerance(t.toGlobalFrameMatrix(), expected, 1e-6f);
}

TEST(TransformableTest, toLocal_shouldGenerateExpected) {
    Transformable t;
    glm::vec3 position = glm::vec3(3, -2, 5);
    glm::vec3 scale = glm::vec3(2, 3, 4);
    Rotation rotation =
        Rotation::fromVec3Rad(glm::vec3(std::numbers::pi_v<float> / 4, std::numbers::pi_v<float> / 2, 0));

    t.setRotation(rotation);
    t.setScale(scale);
    t.setPosition(position);
    t.update();

    glm::mat4 expected = glm::scale(glm::identity<glm::mat4>(), 1.0f / scale);
    expected = glm::rotate(expected, -rotation.x.asRad(), glm::vec3(1, 0, 0));
    expected = glm::rotate(expected, -rotation.y.asRad(), glm::vec3(0, 1, 0));
    expected = glm::rotate(expected, -rotation.z.asRad(), glm::vec3(0, 0, 1));
    expected = glm::translate(expected, -position);

    assertMatricesEqualTolerance(t.toLocalFrameMatrix(), expected, 1e-6f);
}
