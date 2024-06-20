#include "task/ThreadPool.h"

#include "gtest/gtest.h"

using namespace cg;

TEST(ThreadPoolTest, post_shouldExecuteTask) {
    bool taskDone = false;
    {
        ThreadPool tp;
        tp.postTask([&taskDone]() {
            taskDone = true;
        });
    }
    EXPECT_TRUE(taskDone);
}

TEST(ThreadPoolTest, threadIndex_shouldReturnExpected) {
    ThreadPool threadPool(1);
    threadPool.postTask([]() {
        EXPECT_EQ(ThreadPool::threadIndex(), 0);
    });
}

#ifndef NDEBUG
TEST(ThreadPoolTest, threadIndex_threadNotInThreadPool_shouldTriggerAssert) {
    ASSERT_DEATH(ThreadPool::threadIndex(), ".*");
}
#endif
