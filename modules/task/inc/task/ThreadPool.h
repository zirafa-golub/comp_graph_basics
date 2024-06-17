#pragma once

#include "task/MessageQueue.h"
#include "task/Task.h"

#include <latch>
#include <thread>

namespace cg {
class ThreadPool {
public:
    ThreadPool(unsigned threadCount = std::thread::hardware_concurrency()) {
        for (unsigned i = 0; i < threadCount; ++i) {
            threads_.emplace_back([this](const std::stop_token& stopToken) { threadTask(stopToken); });
        }
    }

    ~ThreadPool() {
        std::latch latch(threads_.size() + 1);
        for (size_t i = 0; i < threads_.size(); ++i) {
            taskQueue_.post([&latch]() { latch.arrive_and_wait(); });
        }
        for (auto& thread : threads_) {
            thread.request_stop();
        }
        latch.count_down();
    }

    template <typename T>
    void postTask(T&& task)
        requires std::invocable<T>
    {
        taskQueue_.post(std::forward<T>(task));
    }
    void postTasks(std::vector<Task>& tasks) { taskQueue_.post(tasks); }
    unsigned threadCount() const { return static_cast<unsigned>(threads_.size()); }

private:
    void threadTask(const std::stop_token& stopToken) {
        while (!stopToken.stop_requested()) {
            Task task = taskQueue_.take();
            if (task) {
                task();
            } else {
                break;
            }
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

    MessageQueue<Task> taskQueue_;
    std::vector<std::jthread> threads_;
};

} // namespace cg
