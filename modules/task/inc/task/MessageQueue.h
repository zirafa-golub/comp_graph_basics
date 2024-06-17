#pragma once

#include <concepts>
#include <condition_variable>
#include <deque>
#include <iterator>
#include <mutex>
#include <optional>

#include "task/Task.h"

namespace cg {
template <typename T, typename Container = std::deque<T>>
class MessageQueue {
    static_assert(std::move_constructible<T>);

public:
    template <typename U>
        requires std::convertible_to<U, T>
    void post(U&& msg) {
        {
            std::scoped_lock lock(mtx_);
            messages_.push_back(std::forward<U>(msg));
        }
        hasMessagesCond_.notify_one();
    }

    void post(std::vector<T>& msgs) {
        {
            std::scoped_lock lock(mtx_);
            messages_.insert(messages_.end(), std::make_move_iterator(msgs.begin()),
                             std::make_move_iterator(msgs.end()));
        }
        hasMessagesCond_.notify_all();
    }

    T take() {
        std::unique_lock<std::mutex> lock(mtx_);
        while (messages_.empty()) {
            hasMessagesCond_.wait(lock);
        }
        return popFront();
    }

    std::optional<T> tryTake() {
        std::scoped_lock lock(mtx_);
        if (!messages_.empty()) {
            return popFront();
        }
        return std::nullopt;
    }

    // For debug and testing use only
    size_t size() {
        std::scoped_lock lock(mtx_);
        return messages_.size();
    }

private:
    T popFront() {
        T msg = std::move(messages_.front());
        messages_.pop_front();
        return msg;
    }

    mutable std::mutex mtx_;
    std::condition_variable hasMessagesCond_;
    Container messages_;
};
} // namespace cg
