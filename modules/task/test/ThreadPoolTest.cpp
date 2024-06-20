#include "task/ThreadPool.h"

#include "gtest/gtest.h"

using namespace cg;

TEST(ThreadPoolTest, post_shouldExecuteTask) {
    bool taskDone = false;
    {
        ThreadPool tp;
        tp.postTask([&taskDone]() { taskDone = true; });
    }
    EXPECT_TRUE(taskDone);
}
