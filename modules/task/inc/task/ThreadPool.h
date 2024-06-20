#pragma once

#include "task/MessageQueue.h"
#include "task/Task.h"

#include < vector>
#include <thread>

namespace cg {
class ThreadPool {
public:
    using ThreadIndex = int;
    static constexpr ThreadIndex invalidIndex = -1;

    ThreadPool(unsigned threadCount = std::thread::hardware_concurrency());
    ~ThreadPool();

    template <TaskCallable T>
    void postTask(T&& task) {
        taskQueue_.post(std::forward<T>(task));
    }
    void postTasks(std::vector<Task>& tasks);
    unsigned threadCount() const;
    static ThreadIndex threadIndex();

private:
    void threadTask(const std::stop_token& stopToken, int threadIndex);
    static void setThreadIndex(ThreadIndex index);

    static thread_local ThreadIndex threadIndex_;

    MessageQueue<Task> taskQueue_;
    std::vector<std::jthread> threads_;
};

} // namespace cg
