#pragma once

#include <cstdint>
#include <vector>

namespace cg {
template <typename Val>
class Average {
public:
    Average(size_t maxEntries)
        : maxEntries_(maxEntries), entries_(maxEntries_), nextEntry_(0), entriesAdded_(0), total_{} {}

    void addEntry(Val val) {
        total_ += val;
        total_ -= entries_[nextEntry_];
        entries_[nextEntry_] = std::move(val);
        if (entriesAdded_ < maxEntries_) {
            ++entriesAdded_;
        }
        nextEntry_ = (nextEntry_ + 1) % maxEntries_;
    }

    Val average() const { return total_ / entriesAdded_; }

private:
    size_t maxEntries_;
    std::vector<Val> entries_;
    unsigned nextEntry_;
    unsigned entriesAdded_;
    Val total_;
};

template <typename Val = float>
Average<Val> averageOf(size_t maxEntries) {
    return Average<Val>(maxEntries);
}
} // namespace cg