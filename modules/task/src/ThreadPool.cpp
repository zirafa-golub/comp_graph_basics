#include "task/ThreadPool.h"

#include <cassert>
#include <latch>

namespace cg {
thread_local ThreadPool::ThreadIndex ThreadPool::threadIndex_ = -1;

ThreadPool::ThreadPool(unsigned threadCount) {
    for (unsigned i = 0; i < threadCount; ++i) {
        threads_.emplace_back(
            [this](const std::stop_token& stopToken, int threadIndex) {
                threadTask(stopToken, threadIndex);
            },
            i);
    }
}

ThreadPool::~ThreadPool() {
    // We need to make sure all stop requests are sent before workers pass the latch, which is why this thread also
    // needs to participate in the latch.
    std::latch latch(threads_.size() + 1);
    for (size_t i = 0; i < threads_.size(); ++i) {
        taskQueue_.post([&latch]() {
            latch.arrive_and_wait();
        });
    }
    for (auto& thread : threads_) {
        thread.request_stop();
    }
    latch.count_down();
}

void ThreadPool::postTasks(std::vector<Task>& tasks) { taskQueue_.post(tasks); }

unsigned ThreadPool::threadCount() const { return static_cast<unsigned>(threads_.size()); }

ThreadPool::ThreadIndex ThreadPool::threadIndex() {
    assert(threadIndex_ != ThreadPool::invalidIndex && "This thread is not part of a thread pool.");
    return threadIndex_;
}

void ThreadPool::setThreadIndex(ThreadIndex index) { threadIndex_ = index; }

void ThreadPool::threadTask(const std::stop_token& stopToken, int threadIndex) {
    setThreadIndex(threadIndex);

    while (!stopToken.stop_requested()) {
        Task task = taskQueue_.take();
        task();
    }

    // Drain the queue
    while (true) {
        auto optTask = taskQueue_.tryTake();
        if (optTask.has_value()) {
            optTask.value()();
        } else {
            break;
        }
    }
}

} // namespace cg
