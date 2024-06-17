#pragma once

#include <chrono>

namespace cg {
template <typename Clock = std::chrono::high_resolution_clock, typename DefaultUnit = std::chrono::microseconds>
class TimeProfiler {
public:
    TimeProfiler() : start(Clock::now()) {}

    template <typename Unit = DefaultUnit>
    Unit time() {
        return std::chrono::duration_cast<Unit>(Clock::now() - start).count();
    }

    void reset() { start = Clock::now(); }

    template <typename Unit = DefaultUnit>
    int64_t timeAndReset() {
        auto now = Clock::now();
        uint64_t time = std::chrono::duration_cast<Unit>(now - start).count();
        start = now;
        return time;
    }

private:
    Clock::time_point start;
};
} // namespace cg
