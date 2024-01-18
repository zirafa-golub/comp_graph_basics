#include "gtest/gtest.h"

#include "core/Color.h"

#include "TestUtils.h"

using namespace cg;

TEST(ColorTest, componentConstructor_componentShouldMatch) {
    constexpr float r = 0.5f, g = 0.3f, b = 0.1f;
    Color test_color{r, g, b};

    EXPECT_FLOAT_EQ(test_color.r(), r);
    EXPECT_FLOAT_EQ(test_color.g(), g);
    EXPECT_FLOAT_EQ(test_color.b(), b);
}

TEST(ColorTest, vecConstructor_componentShouldMatch) {
    constexpr float r = 0.5f, g = 0.3f, b = 0.1f;
    Color test_color{glm::vec3{r, g, b}};

    EXPECT_FLOAT_EQ(test_color.r(), r);
    EXPECT_FLOAT_EQ(test_color.g(), g);
    EXPECT_FLOAT_EQ(test_color.b(), b);
}

TEST(ColorTest, opEqual_equalColors_shouldReturnExpected) {
    constexpr float r = 0.5f, g = 0.3f, b = 0.1f;
    Color color1{r, g, b};
    Color color2{r, g, b};

    EXPECT_EQ(color1, color2);
    EXPECT_FALSE(color1 != color2);
}

TEST(ColorTest, opEqual_differentColors_shouldReturnFalse) {
    Color color1{0.5f, 0.3f, 0.1f};
    Color color2{0.5f, 0.3f, 0.2f};

    EXPECT_FALSE(color1 == color2);
    EXPECT_NE(color1, color2);
}

TEST(ColorTest, opAdd_withColor_shouldReturnExpected) {
    Color color1{glm::vec3{0.3f, 0.4f, 0.5f}};
    Color color2{glm::vec3{0.1f, 0.5f, 0.2f}};

    assertColorsFloatEqual(color1 + color2, Color(0.4f, 0.9f, 0.7f));
}

TEST(ColorTest, opSubtract_withColor_shouldReturnExpected) {
    Color color1{glm::vec3{0.3f, 0.5f, 0.5f}};
    Color color2{glm::vec3{0.1f, 0.4f, 0.2f}};

    Color c = color1 - color2;
    float f = 0.2f;

    assertColorsFloatEqual(color1 - color2, Color(0.2f, 0.1f, 0.3f));
}

TEST(ColorTest, opMultiply_withColor_shouldReturnExpected) {
    Color color1{glm::vec3{0.3f, 0.8f, 0.5f}};
    Color color2{glm::vec3{0.3f, 0.5f, 0.2f}};

    assertColorsFloatEqual(color1 * color2, Color(0.09f, 0.4f, 0.1f));
}

TEST(ColorTest, opMultiply_withFloat_shouldReturnExpected) {
    Color test_color{0.3f, 0.4f, 0.2f};

    assertColorsFloatEqual(test_color * 2.0f, Color(0.6f, 0.8f, 0.4f));
    assertColorsFloatEqual(2.0f * test_color, Color(0.6f, 0.8f, 0.4f));
}

TEST(ColorTest, opDivide_withFloat_shouldReturnExpected) {
    Color test_color{0.6f, 0.4f, 0.2f};

    assertColorsFloatEqual(test_color / 2.0f, Color(0.3f, 0.2f, 0.1f));
}