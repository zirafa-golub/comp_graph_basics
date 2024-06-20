#pragma once

#include "task/Task.h"
#include "task/ThreadPool.h"

#include <atomic>
#include <cassert>
#include <concepts>
#include <future>
#include <vector>

namespace cg {
template <typename T>
concept TaskGenerator = std::invocable<T> && std::invocable<std::invoke_result_t<T>>;

namespace detail {
struct TaskGraphArchetype {
    void start(ThreadPool&);
    template <TaskCallable Continuation>
    void start(ThreadPool&, Continuation&& cont);
    void startAndWait(ThreadPool&);
    template <TaskCallable Task>
    void addWork(Task&&);
    void addWork(TaskGraphArchetype&&);
    template <TaskGenerator Gen>
    void addDynamicWork(Gen&&);
    void addDynamicWork(std::move_only_function<TaskGraphArchetype(void)>&&);
};
} // namespace detail

template <typename T>
concept TaskGraph = requires(T taskGraph, ThreadPool& threadPool, std::move_only_function<void(void)> task,
                             detail::TaskGraphArchetype subGraph, std::move_only_function<Task(void)> taskGenerator,
                             std::move_only_function<detail::TaskGraphArchetype(void)> graphGenerator) {
    { taskGraph.start(threadPool) } -> std::same_as<void>;
    { taskGraph.start(threadPool, std::move(task)) } -> std::same_as<void>;
    { taskGraph.startAndWait(threadPool) } -> std::same_as<void>;
    { taskGraph.addWork(std::move(task)) } -> std::same_as<void>;
    { taskGraph.addWork(std::move(subGraph)) } -> std::same_as<void>;
    { taskGraph.addDynamicWork(std::move(taskGenerator)) } -> std::same_as<void>;
    { taskGraph.addDynamicWork(std::move(graphGenerator)) } -> std::same_as<void>;
};

static_assert(TaskGraph<detail::TaskGraphArchetype>);

template <typename T>
concept GraphGenerator = std::invocable<T> && TaskGraph<std::invoke_result_t<T>>;

namespace detail {
class WhenAllHelper {
public:
    void markTaskDone() {
        size_t tasksLeft = --tasksLeft_;
        if (tasksLeft == 0) {
            // This way, if continuation owns the helper, it will clean it up once executed
            Task tmpCont = std::move(continuation_);
            tmpCont();
        }
    }

    template <TaskCallable T>
    void setContinuation(T&& continuation) {
        continuation_ = std::forward<T>(continuation);
    }
    void setTaskCount(size_t taskCount) { tasksLeft_ = taskCount; }

private:
    std::atomic<size_t> tasksLeft_;
    Task continuation_;
};
} // namespace detail

class TaskBatch {
public:
    TaskBatch() : state_(std::make_unique<State>()) {}

    template <TaskCallable T>
    void addWork(T&& task) {
        tasks_.emplace_back([state = state_.get(), batchedTask(std::forward<T>(task))]() mutable {
            batchedTask();
            state->whenAllHelper.markTaskDone();
        });
    }

    template <TaskGraph SubGraph>
    void addWork(SubGraph&& subGraph) {
        tasks_.emplace_back([state = state_.get(), subGraph = std::move(subGraph)]() mutable {
            subGraph.start(*state->threadPool, [state]() {
                state->whenAllHelper.markTaskDone();
            });
        });
    }

    template <TaskGenerator Gen>
    void addDynamicWork(Gen&& generator) {
        addWork([gen(std::forward<Gen>(generator))]() {
            auto task = gen();
            task();
        });
    }

    template <GraphGenerator Gen>
    void addDynamicWork(Gen&& generator) {
        tasks_.emplace_back([state = state_.get(), gen(std::forward<Gen>(generator))]() mutable {
            auto subGraph = gen();
            subGraph.start(*state->threadPool, [state]() {
                state->whenAllHelper.markTaskDone();
            });
        });
    }

    void start(ThreadPool& threadPool) {
        start(threadPool, []() {});
    }

    template <TaskCallable T>
    void start(ThreadPool& threadPool, T&& continuation) {
        state_->whenAllHelper.setTaskCount(tasks_.size());
        state_->threadPool = &threadPool;
        detail::WhenAllHelper& helper = state_->whenAllHelper;
        helper.setContinuation([cont(std::forward<T>(continuation)), state = std::move(state_)]() mutable {
            cont();
        });
        threadPool.postTasks(tasks_);
    }

    void startAndWait(ThreadPool& threadPool) {
        std::promise<void> promise;
        auto future = promise.get_future();
        start(threadPool, [promise = std::move(promise)]() mutable {
            promise.set_value();
        });
        future.get();
    }

private:
    struct State {
        detail::WhenAllHelper whenAllHelper;
        ThreadPool* threadPool;
    };

    std::vector<Task> tasks_;
    std::unique_ptr<State> state_;
};

static_assert(TaskGraph<TaskBatch>, "TaskBatch does not fulfill the TaskGraph concept.");

class TaskSequence {
public:
    TaskSequence() : state_(std::make_unique<State>()) {}

    template <TaskCallable T>
    void addWork(T&& task) {
        state_->tasks.emplace_back([task(std::forward<T>(task)), state = state_.get()]() mutable {
            task();
            state->moveToNextTask();
        });
    }

    template <TaskGraph SubGraph>
    void addWork(SubGraph&& subGraph) {
        state_->tasks.emplace_back([subGraph(std::forward<SubGraph>(subGraph)), state = state_.get()]() mutable {
            subGraph.start(*state->threadPool, [state]() mutable {
                state->moveToNextTask();
            });
        });
    }

    template <TaskGenerator Gen>
    void addDynamicWork(Gen&& generator) {
        addWork([gen(std::forward<Gen>(generator))]() {
            auto task = gen();
            task();
        });
    }

    template <GraphGenerator Gen>
    void addDynamicWork(Gen&& generator) {
        state_->tasks.emplace_back([state = state_.get(), gen(std::forward<Gen>(generator))]() mutable {
            auto subGraph = gen();
            subGraph.start(*state->threadPool, [state]() {
                state->moveToNextTask();
            });
        });
    }

    void start(ThreadPool& threadPool) {
        start(threadPool, []() {});
    }

    template <TaskCallable T>
    void start(ThreadPool& threadPool, T&& continuation) {
        state_->threadPool = &threadPool;
        State* statePtr = state_.get();
        statePtr->tasks.emplace_back([cont(std::forward<T>(continuation)), state = std::move(state_)]() mutable {
            cont();
        });
        threadPool.postTask([state = statePtr]() {
            state->moveToNextTask();
        });
    }

    void startAndWait(ThreadPool& threadPool) {
        std::promise<void> promise;
        auto future = promise.get_future();
        start(threadPool, [promise = std::move(promise)]() mutable {
            promise.set_value();
        });
        future.get();
    }

private:
    struct State {
        std::vector<Task> tasks;
        unsigned currTask_ = 0;
        ThreadPool* threadPool;

        void moveToNextTask() {
            assert(currTask_ < tasks.size());
            // Remove task from the container, so it's cleaned up after it's executed
            auto tempTask = std::move(tasks[currTask_++]);
            tempTask();
        }
    };

    std::unique_ptr<State> state_;
};

static_assert(TaskGraph<TaskSequence>, "TaskSequence does not fulfill the TaskGraph concept.");
} // namespace cg
