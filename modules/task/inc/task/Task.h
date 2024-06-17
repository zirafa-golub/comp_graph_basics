#pragma once

#include <functional>

namespace cg {
using Task = std::move_only_function<void()>;
} // namespace cg
