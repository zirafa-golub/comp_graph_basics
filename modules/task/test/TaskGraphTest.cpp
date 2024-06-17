#include "task/TaskGraph.h"

#include "gtest/gtest.h"

#include <array>
#include <vector>

using namespace cg;

TEST(TaskBatchTest, addTaskAndStart_multipleTasks_allShouldExecute) {
    TaskBatch batch;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount, 0);
    for (int i = 0; i < taskCount; ++i) {
        batch.addTask([i, &results]() { results[i] = i + 1; });
    }
    {
        ThreadPool threadPool;
        batch.start(threadPool);
    }

    EXPECT_EQ(results, std::vector<int>({1, 2, 3}));
}

TEST(TaskBatchTest, addTaskAndStart_multipleTasks_allShouldExecuteAndTriggerContinuation) {
    TaskBatch batch;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount + 1, 0);
    for (int i = 0; i < taskCount; ++i) {
        batch.addTask([i, &results]() { results[i] = i + 1; });
    }
    {
        ThreadPool threadPool;
        batch.start(threadPool, [&results]() {
            EXPECT_EQ(results, std::vector<int>({1, 2, 3, 0}));
            results[taskCount] = taskCount + 1;
        });
    }

    EXPECT_EQ(results, std::vector<int>({1, 2, 3, 4}));
}

TEST(TaskBatchTest, addTaskAndStartAndWait_multipleTasks_allShouldExecuteBeforeWaitEnds) {
    TaskBatch batch;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount, 0);
    for (int i = 0; i < taskCount; ++i) {
        batch.addTask([i, &results]() { results[i] = i + 1; });
    }
    ThreadPool threadPool;
    batch.startAndWait(threadPool);
    EXPECT_EQ(results, std::vector<int>({1, 2, 3}));
}

TEST(TaskBatchTest, addSubGraphAndStartAndWait_batchSubGraphs_allShouldExecuteBeforeWaitEnds) {
    constexpr int batchCount = 3;
    std::vector<std::vector<int>> results(batchCount);
    constexpr std::array<int, batchCount> taskCount = {2, 4, 3};

    TaskBatch mainBatch;

    for (int i = 0; i < results.size(); ++i) {
        results[i].resize(taskCount[i], 0);
        TaskBatch subBatch;
        for (int j = 0; j < results[i].size(); ++j) {
            subBatch.addTask([&results, i, j]() { results[i][j] = i + j + 1; });
        }
        mainBatch.addSubGraph(std::move(subBatch));
    }
    ThreadPool threadPool;
    mainBatch.startAndWait(threadPool);

    EXPECT_EQ(results[0], std::vector<int>({1, 2}));
    EXPECT_EQ(results[1], std::vector<int>({2, 3, 4, 5}));
    EXPECT_EQ(results[2], std::vector<int>({3, 4, 5}));
}

TEST(TaskBatchTest, addSubGraphAndStartAndWait_sequenceSubGraphs_allShouldExecuteBeforeWaitEnds) {
    constexpr int batchCount = 3;
    std::vector<std::vector<int>> results(batchCount);
    constexpr std::array<int, batchCount> taskCount = {2, 4, 3};

    TaskBatch batch;

    for (int i = 0; i < results.size(); ++i) {
        results[i].resize(taskCount[i], 0);
        TaskSequence subSequence;
        for (int j = 0; j < results[i].size(); ++j) {
            subSequence.addTask([&results, i, j]() { results[i][j] = i + j + 1; });
        }
        batch.addSubGraph(std::move(subSequence));
    }
    ThreadPool threadPool;
    batch.startAndWait(threadPool);

    EXPECT_EQ(results[0], std::vector<int>({1, 2}));
    EXPECT_EQ(results[1], std::vector<int>({2, 3, 4, 5}));
    EXPECT_EQ(results[2], std::vector<int>({3, 4, 5}));
}

TEST(TaskSequenceTest, addTaskAndStart_multipleTasks_allShouldExecute) {
    TaskSequence seq;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount, 0);
    std::vector<std::vector<int>> expected = {{0, 0, 0}, {1, 0, 0}, {1, 2, 0}};

    for (int i = 0; i < taskCount; ++i) {
        seq.addTask([i, &results, &expected]() {
            EXPECT_EQ(results, expected[i]);
            results[i] = i + 1;
        });
    }
    {
        ThreadPool threadPool;
        seq.start(threadPool);
    }

    EXPECT_EQ(results, std::vector<int>({1, 2, 3}));
}

TEST(TaskSequenceTest, addTaskAndStart_multipleTasks_allShouldExecuteAndTriggerContinuation) {
    TaskSequence seq;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount + 1, 0);
    std::vector<std::vector<int>> expected = {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 2, 0, 0}, {1, 2, 3, 0}};

    for (int i = 0; i < taskCount; ++i) {
        seq.addTask([i, &results, &expected]() {
            EXPECT_EQ(results, expected[i]);
            results[i] = i + 1;
        });
    }
    {
        ThreadPool threadPool;
        seq.start(threadPool, [&results, &expected]() {
            EXPECT_EQ(results, expected[taskCount]);
            results[taskCount] = taskCount + 1;
        });
    }

    EXPECT_EQ(results, std::vector<int>({1, 2, 3, 4}));
}

TEST(TaskSequenceTest, addTaskAndStartAndWait_multipleTasks_allShouldExecuteBeforeWaitEnds) {
    TaskSequence seq;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount, 0);
    std::vector<std::vector<int>> expected = {{0, 0, 0}, {1, 0, 0}, {1, 2, 0}};

    for (int i = 0; i < taskCount; ++i) {
        seq.addTask([i, &results, &expected]() {
            EXPECT_EQ(results, expected[i]);
            results[i] = i + 1;
        });
    }
    ThreadPool threadPool;
    seq.startAndWait(threadPool);

    EXPECT_EQ(results, std::vector<int>({1, 2, 3}));
}

TEST(TaskSequenceTest, addSubGraphAndStartAndWait_sequenceSubGraphs_allShouldExecuteBeforeWaitEnds) {
    TaskSequence mainSeq;

    std::vector<int> results;
    constexpr int seqCount = 3;
    constexpr int taskCount = 2;
    results.resize(seqCount * taskCount, 0);
    std::vector<std::vector<int>> expected = {{0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 2, 0, 0, 0, 0},
                                              {1, 2, 3, 0, 0, 0}, {1, 2, 3, 4, 0, 0}, {1, 2, 3, 4, 5, 0}};

    for (int i = 0; i < seqCount; ++i) {
        TaskSequence subSeq;
        for (int j = 0; j < taskCount; ++j) {
            subSeq.addTask([index = i * taskCount + j, &results, &expected]() {
                EXPECT_EQ(results, expected[index]);
                results[index] = index + 1;
            });
        }
        mainSeq.addSubGraph(std::move(subSeq));
    }
    ThreadPool threadPool;
    mainSeq.startAndWait(threadPool);

    EXPECT_EQ(results, std::vector<int>({1, 2, 3, 4, 5, 6}));
}

TEST(TaskSequenceTest, addSubGraphAndStartAndWait_batchSubGraphs_allShouldExecuteBeforeWaitEnds) {
    TaskSequence seq;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(2 * taskCount, 0);
    std::vector<int> expected = {1, 2, 3, 0, 0, 0};

    TaskBatch subBatch1;
    for (int i = 0; i < taskCount; ++i) {
        subBatch1.addTask([i, &results, &expected]() { results[i] = i + 1; });
    }
    seq.addSubGraph(std::move(subBatch1));

    seq.addTask([&results, &expected]() { EXPECT_EQ(results, expected); });

    TaskBatch subBatch2;
    for (int i = 0; i < taskCount; ++i) {
        subBatch2.addTask([index = taskCount + i, &results, &expected]() { results[index] = index + 1; });
    }
    seq.addSubGraph(std::move(subBatch2));

    ThreadPool threadPool;
    seq.startAndWait(threadPool);

    EXPECT_EQ(results, std::vector<int>({1, 2, 3, 4, 5, 6}));
}
