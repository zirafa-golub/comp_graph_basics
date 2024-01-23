#include "common/Average.h"

#include "gtest/gtest.h"

using namespace cg;

TEST(AverageTest, average_lessThanMaxEntries_shouldReturnAverageSoFar) {
    auto averageCalc = averageOf<uint64_t>(5);
    averageCalc.addEntry(4);
    averageCalc.addEntry(6);
    averageCalc.addEntry(5);

    EXPECT_EQ(averageCalc.average(), 5);
}

TEST(AverageTest, average_maxEntries_shouldReturnAverageSoFar) {
    auto averageCalc = averageOf<uint64_t>(5);
    averageCalc.addEntry(4);
    averageCalc.addEntry(6);
    averageCalc.addEntry(5);
    averageCalc.addEntry(6);
    averageCalc.addEntry(5);

    EXPECT_EQ(averageCalc.average(), 5);
}

TEST(AverageTest, average_moreThanMaxEntries_shouldReturnAverageForMaxLastEntries) {
    auto averageCalc = averageOf<uint64_t>(5);
    averageCalc.addEntry(4);
    averageCalc.addEntry(6);
    averageCalc.addEntry(5);
    averageCalc.addEntry(6);
    averageCalc.addEntry(5);
    averageCalc.addEntry(10);

    EXPECT_EQ(averageCalc.average(), 6);
}