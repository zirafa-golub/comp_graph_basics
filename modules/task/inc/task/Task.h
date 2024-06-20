#pragma once

#include <concepts>
#include <functional>

namespace cg {
using Task = std::move_only_function<void()>;

template <typename T>
concept TaskCallable = std::convertible_to<T, Task>;
} // namespace cg
