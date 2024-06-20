#pragma once

#include <chrono>

namespace cg {
template <typename Clock = std::chrono::high_resolution_clock, typename DefaultUnit = std::chrono::microseconds>
class TimeProfiler {
public:
    TimeProfiler() : start(Clock::now()) {}

    template <typename Unit = DefaultUnit>
    Unit time() const {
        return std::chrono::duration_cast<Unit>(Clock::now() - start);
    }

    void reset() { start = Clock::now(); }

    template <typename Unit = DefaultUnit>
    Unit timeAndReset() {
        auto now = Clock::now();
        Unit time = std::chrono::duration_cast<Unit>(now - start);
        start = now;
        return time;
    }

private:
    Clock::time_point start;
};
} // namespace cg
