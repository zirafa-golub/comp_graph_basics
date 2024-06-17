#include "task/MessageQueue.h"

#include "gtest/gtest.h"

using namespace cg;

TEST(MessageQueueTest, submitTakeSize_shouldReturnExpected) {
    MessageQueue<int> messageQueue;
    constexpr int val = 10;

    EXPECT_EQ(messageQueue.size(), 0);
    messageQueue.post(val);
    messageQueue.post(val + 1);
    EXPECT_EQ(messageQueue.size(), 2);
    int taken = messageQueue.take();
    EXPECT_EQ(taken, val);
    taken = messageQueue.take();
    EXPECT_EQ(taken, val + 1);
    EXPECT_EQ(messageQueue.size(), 0);
}

TEST(MessageQueueTest, tryTake_emptyQueue_shouldReturnEmpty) {
    MessageQueue<int> messageQueue;
    auto optMsg = messageQueue.tryTake();
    EXPECT_FALSE(optMsg.has_value());
}

TEST(MessageQueueTest, tryTake_hasMessage_shouldReturnMessage) {
    MessageQueue<int> messageQueue;
    constexpr int val = 10;
    messageQueue.post(val);
    auto optMsg = messageQueue.tryTake();
    ASSERT_TRUE(optMsg.has_value());
    EXPECT_EQ(optMsg.value(), val);
}
