#include "rasterizer/PlaneIntersect.h"

#include "gtest/gtest.h"

using namespace cg;

TEST(PlaneIntersectTest, isVertexAbove_vertexAbove_shouldReturnTrue) {
    PlaneIntersect plane({2, 0, 1}, {3, 0, -1}, {-2, 0, -2});
    Point vertex(4, 5, 6);

    EXPECT_TRUE(plane.isVertexAbove(vertex));
}

TEST(PlaneIntersectTest, isVertexAbove_vertexBelow_shouldReturnFalse) {
    PlaneIntersect plane({2, 0, 1}, {3, 0, -1}, {-2, 0, -2});
    Point vertex(4, -5, 6);

    EXPECT_FALSE(plane.isVertexAbove(vertex));
}

TEST(PlaneIntersectTest, isVertexAbove_vertexOnPlane_shouldReturnFalse) {
    PlaneIntersect plane({2, 0, 1}, {3, 0, -1}, {-2, 0, -2});
    Point vertex(4, 0, 6);

    EXPECT_FALSE(plane.isVertexAbove(vertex));
}

TEST(PlaneIntersectTest, intersectSegment_pointsOnDifferentSides_shouldReturnExpected) {
    PlaneIntersect plane({2, 0, 1}, {3, 0, -1}, {-2, 0, -2});

    auto result = plane.intersectSegment(Point(6, 5, 3), Point(6, -3, 3));
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().intersection, Point(6, 0, 3));
    EXPECT_FLOAT_EQ(result.value().fromFirstToSecond, 0.625f);
}

TEST(PlaneIntersectTest, intersectSegment_pointsOnSameSide_shouldReturnNoResult) {
    PlaneIntersect plane({2, 0, 1}, {3, 0, -1}, {-2, 0, -2});

    EXPECT_EQ(plane.intersectSegment(Point(6, 5, 3), Point(6, 2, 3)), std::nullopt);
}

TEST(PlaneIntersectTest, intersectSegment_segmentParallelToPlane_shouldReturnNoResult) {
    PlaneIntersect plane({2, 0, 1}, {3, 0, -1}, {-2, 0, -2});

    EXPECT_EQ(plane.intersectSegment(Point(6, 3, 3), Point(4, 3, -2)), std::nullopt);
}
