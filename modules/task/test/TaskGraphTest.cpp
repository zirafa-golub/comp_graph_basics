#include "task/TaskGraph.h"

#include "gtest/gtest.h"

#include <array>
#include <vector>

using namespace cg;

TEST(TaskBatchTest, addWorkAndStart_multipleTasks_allShouldExecute) {
    TaskBatch batch;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount, 0);
    for (int i = 0; i < taskCount; ++i) {
        batch.addWork([i, &results]() {
            results[i] = i + 1;
        });
    }
    {
        ThreadPool threadPool;
        batch.start(threadPool);
    }

    EXPECT_EQ(results, std::vector<int>({1, 2, 3}));
}

TEST(TaskBatchTest, addWorkAndStart_multipleTasks_allShouldExecuteAndTriggerContinuation) {
    TaskBatch batch;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount + 1, 0);
    for (int i = 0; i < taskCount; ++i) {
        batch.addWork([i, &results]() {
            results[i] = i + 1;
        });
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

TEST(TaskBatchTest, addWorkAndStartAndWait_multipleTasks_allShouldExecuteBeforeWaitEnds) {
    TaskBatch batch;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount, 0);
    for (int i = 0; i < taskCount; ++i) {
        batch.addWork([i, &results]() {
            results[i] = i + 1;
        });
    }
    ThreadPool threadPool;
    batch.startAndWait(threadPool);
    EXPECT_EQ(results, std::vector<int>({1, 2, 3}));
}

TEST(TaskBatchTest, addWorkAndStartAndWait_batchSubGraphs_allShouldExecuteBeforeWaitEnds) {
    constexpr int batchCount = 3;
    std::vector<std::vector<int>> results(batchCount);
    constexpr std::array<int, batchCount> taskCount = {2, 4, 3};

    TaskBatch mainBatch;

    for (int i = 0; i < results.size(); ++i) {
        results[i].resize(taskCount[i], 0);
        TaskBatch subBatch;
        for (int j = 0; j < results[i].size(); ++j) {
            subBatch.addWork([&results, i, j]() {
                results[i][j] = i + j + 1;
            });
        }
        mainBatch.addWork(std::move(subBatch));
    }
    ThreadPool threadPool;
    mainBatch.startAndWait(threadPool);

    EXPECT_EQ(results[0], std::vector<int>({1, 2}));
    EXPECT_EQ(results[1], std::vector<int>({2, 3, 4, 5}));
    EXPECT_EQ(results[2], std::vector<int>({3, 4, 5}));
}

TEST(TaskBatchTest, addWorkAndStartAndWait_sequenceSubGraphs_allShouldExecuteBeforeWaitEnds) {
    constexpr int batchCount = 3;
    std::vector<std::vector<int>> results(batchCount);
    constexpr std::array<int, batchCount> taskCount = {2, 4, 3};

    TaskBatch batch;

    for (int i = 0; i < results.size(); ++i) {
        results[i].resize(taskCount[i], 0);
        TaskSequence subSequence;
        for (int j = 0; j < results[i].size(); ++j) {
            subSequence.addWork([&results, i, j]() {
                results[i][j] = i + j + 1;
            });
        }
        batch.addWork(std::move(subSequence));
    }
    ThreadPool threadPool;
    batch.startAndWait(threadPool);

    EXPECT_EQ(results[0], std::vector<int>({1, 2}));
    EXPECT_EQ(results[1], std::vector<int>({2, 3, 4, 5}));
    EXPECT_EQ(results[2], std::vector<int>({3, 4, 5}));
}

TEST(TaskBatchTest, addDynamicWork_generateTask_allShouldExecuteBeforeWaitEnds) {
    TaskBatch batch;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount, 0);
    for (int i = 0; i < taskCount; ++i) {
        batch.addDynamicWork([i, &results]() {
            return [i, &results]() {
                results[i] = i + 1;
            };
        });
    }
    ThreadPool threadPool;
    batch.startAndWait(threadPool);
    EXPECT_EQ(results, std::vector<int>({1, 2, 3}));
}

TEST(TaskBatchTest, addDynamicWork_generateSubGraph_allShouldExecuteBeforeWaitEnds) {
    constexpr int batchCount = 3;
    std::vector<std::vector<int>> results(batchCount);
    constexpr std::array<int, batchCount> taskCount = {2, 4, 3};

    TaskBatch mainBatch;

    for (int i = 0; i < results.size(); ++i) {
        results[i].resize(taskCount[i], 0);
        mainBatch.addDynamicWork([&results, i]() {
            TaskBatch subBatch;
            for (int j = 0; j < results[i].size(); ++j) {
                subBatch.addWork([&results, i, j]() {
                    results[i][j] = i + j + 1;
                });
            }
            return subBatch;
        });
    }
    ThreadPool threadPool;
    mainBatch.startAndWait(threadPool);

    EXPECT_EQ(results[0], std::vector<int>({1, 2}));
    EXPECT_EQ(results[1], std::vector<int>({2, 3, 4, 5}));
    EXPECT_EQ(results[2], std::vector<int>({3, 4, 5}));
}

TEST(TaskSequenceTest, addWorkAndStart_multipleTasks_allShouldExecute) {
    TaskSequence seq;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount, 0);
    std::vector<std::vector<int>> expected = {{0, 0, 0}, {1, 0, 0}, {1, 2, 0}};

    for (int i = 0; i < taskCount; ++i) {
        seq.addWork([i, &results, &expected]() {
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

TEST(TaskSequenceTest, addWorkAndStart_multipleTasks_allShouldExecuteAndTriggerContinuation) {
    TaskSequence seq;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount + 1, 0);
    std::vector<std::vector<int>> expected = {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 2, 0, 0}, {1, 2, 3, 0}};

    for (int i = 0; i < taskCount; ++i) {
        seq.addWork([i, &results, &expected]() {
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

TEST(TaskSequenceTest, addWorkAndStartAndWait_multipleTasks_allShouldExecuteBeforeWaitEnds) {
    TaskSequence seq;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount, 0);
    std::vector<std::vector<int>> expected = {{0, 0, 0}, {1, 0, 0}, {1, 2, 0}};

    for (int i = 0; i < taskCount; ++i) {
        seq.addWork([i, &results, &expected]() {
            EXPECT_EQ(results, expected[i]);
            results[i] = i + 1;
        });
    }
    ThreadPool threadPool;
    seq.startAndWait(threadPool);

    EXPECT_EQ(results, std::vector<int>({1, 2, 3}));
}

TEST(TaskSequenceTest, addWorkAndStartAndWait_sequenceSubGraphs_allShouldExecuteBeforeWaitEnds) {
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
            subSeq.addWork([index = i * taskCount + j, &results, &expected]() {
                EXPECT_EQ(results, expected[index]);
                results[index] = index + 1;
            });
        }
        mainSeq.addWork(std::move(subSeq));
    }
    ThreadPool threadPool;
    mainSeq.startAndWait(threadPool);

    EXPECT_EQ(results, std::vector<int>({1, 2, 3, 4, 5, 6}));
}

TEST(TaskSequenceTest, addWorkAndStartAndWait_batchSubGraphs_allShouldExecuteBeforeWaitEnds) {
    TaskSequence seq;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(2 * taskCount, 0);
    std::vector<int> expected = {1, 2, 3, 0, 0, 0};

    TaskBatch subBatch1;
    for (int i = 0; i < taskCount; ++i) {
        subBatch1.addWork([i, &results, &expected]() {
            results[i] = i + 1;
        });
    }
    seq.addWork(std::move(subBatch1));

    seq.addWork([&results, &expected]() {
        EXPECT_EQ(results, expected);
    });

    TaskBatch subBatch2;
    for (int i = 0; i < taskCount; ++i) {
        subBatch2.addWork([index = taskCount + i, &results, &expected]() {
            results[index] = index + 1;
        });
    }
    seq.addWork(std::move(subBatch2));

    ThreadPool threadPool;
    seq.startAndWait(threadPool);

    EXPECT_EQ(results, std::vector<int>({1, 2, 3, 4, 5, 6}));
}

TEST(TaskSequenceTest, addDynamicWork_generateTask_allShouldExecuteBeforeWaitEnds) {
    TaskSequence seq;

    std::vector<int> results;
    constexpr int taskCount = 3;
    results.resize(taskCount, 0);
    std::vector<std::vector<int>> expected = {{0, 0, 0}, {1, 0, 0}, {1, 2, 0}};

    for (int i = 0; i < taskCount; ++i) {
        seq.addDynamicWork([i, &results, &expected]() {
            return [i, &results, &expected]() {
                EXPECT_EQ(results, expected[i]);
                results[i] = i + 1;
            };
        });
    }
    ThreadPool threadPool;
    seq.startAndWait(threadPool);

    EXPECT_EQ(results, std::vector<int>({1, 2, 3}));
}

TEST(TaskSequenceTest, addDynamicWork_generateSubGraph_allShouldExecuteBeforeWaitEnds) {
    TaskSequence mainSeq;

    std::vector<int> results;
    constexpr int seqCount = 3;
    constexpr int taskCount = 2;
    results.resize(seqCount * taskCount, 0);
    std::vector<std::vector<int>> expected = {{0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 2, 0, 0, 0, 0},
                                              {1, 2, 3, 0, 0, 0}, {1, 2, 3, 4, 0, 0}, {1, 2, 3, 4, 5, 0}};

    for (int i = 0; i < seqCount; ++i) {
        mainSeq.addDynamicWork([i, &results, &expected]() {
            TaskSequence subSeq;
            for (int j = 0; j < taskCount; ++j) {
                subSeq.addWork([index = i * taskCount + j, &results, &expected]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    EXPECT_EQ(results, expected[index]);
                    results[index] = index + 1;
                });
            }
            return subSeq;
        });
    }
    ThreadPool threadPool;
    mainSeq.startAndWait(threadPool);
    EXPECT_EQ(results, std::vector<int>({1, 2, 3, 4, 5, 6}));
}
