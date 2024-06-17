#pragma once

#include "task/Task.h"
#include "task/ThreadPool.h"

#include <atomic>
#include <concepts>
#include <future>
#include <vector>

namespace cg {

namespace detail {
struct TaskGraphArchetype {
    void start(ThreadPool&);
    template <typename Continuation>
    void start(ThreadPool&, Continuation&& cont);
    void startAndWait(ThreadPool&);
    template <typename Task>
    void addTask(Task&&);
    void addSubGraph(TaskGraphArchetype&&);
};
} // namespace detail

template <typename T>
concept TaskGraph = requires(T taskGraph, ThreadPool& threadPool, std::move_only_function<void(void)> task,
                             detail::TaskGraphArchetype subGraph) {
    { taskGraph.start(threadPool) } -> std::same_as<void>;
    { taskGraph.start(threadPool, task) } -> std::same_as<void>;
    { taskGraph.startAndWait(threadPool) } -> std::same_as<void>;
    { taskGraph.addTask(task) } -> std::same_as<void>;
    { taskGraph.addSubGraph(std::move(subGraph)) } -> std::same_as<void>;
};

static_assert(TaskGraph<detail::TaskGraphArchetype>);

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

    template <typename T>
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

    template <typename T>
    void addTask(T&& task) {
        tasks_.emplace_back([state = state_.get(), batchedTask(std::forward<T>(task))]() mutable {
            batchedTask();
            state->whenAllHelper.markTaskDone();
        });
    }

    template <TaskGraph SubGraph>
    void addSubGraph(SubGraph&& subGraph) {
        tasks_.emplace_back([state = state_.get(), subGraph = std::move(subGraph)]() mutable {
            subGraph.start(*state->threadPool, [state]() { state->whenAllHelper.markTaskDone(); });
        });
    }

    void start(ThreadPool& threadPool) {
        start(threadPool, []() {});
    }

    template <typename T>
    void start(ThreadPool& threadPool, T&& continuation) {
        state_->whenAllHelper.setTaskCount(tasks_.size());
        state_->threadPool = &threadPool;
        detail::WhenAllHelper& helper = state_->whenAllHelper;
        helper.setContinuation([cont(std::forward<T>(continuation)), state = std::move(state_)]() mutable { cont(); });
        threadPool.postTasks(tasks_);
    }

    void startAndWait(ThreadPool& threadPool) {
        std::promise<void> promise;
        auto future = promise.get_future();
        start(threadPool, [promise = std::move(promise)]() mutable { promise.set_value(); });
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

    void start(ThreadPool& threadPool) {
        start(threadPool, []() {});
    }

    template <typename T>
    void start(ThreadPool& threadPool, T&& continuation) {
        // TODO: Differentiate between (normal) tasks and subgraph tasks so we can do the first ones in a for loop (on
        // thread pool) so we don't keep expanding stack?
        state_->threadPool = &threadPool;
        State* statePtr = state_.get();
        addTaskToState(statePtr,
                       [cont(std::forward<T>(continuation)), state = std::move(state_)]() mutable { cont(); });
        threadPool.postTask([state = statePtr]() { state->moveToNextTask(); });
    }

    template <typename T>
    void addTask(T&& task) {
        addTaskToState(state_.get(), [state = state_.get(), clientTask(std::forward<T>(task))]() mutable {
            clientTask();
            state->moveToNextTask();
        });
    }

    template <TaskGraph SubGraph>
    void addSubGraph(SubGraph&& subGraph) {
        state_->tasks.emplace_back([subGraph(std::forward<SubGraph>(subGraph)), state = state_.get()]() mutable {
            subGraph.start(*state->threadPool, [state]() mutable { state->moveToNextTask(); });
        });
    }

    void startAndWait(ThreadPool& threadPool) {
        std::promise<void> promise;
        auto future = promise.get_future();
        start(threadPool, [promise = std::move(promise)]() mutable { promise.set_value(); });
        future.get();
    }

private:
    struct State {
        std::vector<Task> tasks;
        unsigned currTask_ = 0;
        ThreadPool* threadPool;

        void moveToNextTask() {
            if (currTask_ < tasks.size()) {
                // Remove task from the container, so it's cleaned up after it's executed
                auto tempTask = std::move(tasks[currTask_++]);
                tempTask();
            }
        }
    };

    template <typename T>
    static void addTaskToState(State* state, T&& task) {
        state->tasks.emplace_back([state, clientTask(std::forward<T>(task))]() mutable {
            clientTask();
            state->moveToNextTask();
        });
    }

    std::unique_ptr<State> state_;
};

static_assert(TaskGraph<TaskSequence>, "TaskSequence does not fulfill the TaskGraph concept.");
} // namespace cg
