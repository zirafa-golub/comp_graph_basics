#pragma once

#include <memory>

namespace cg {
template <typename T>
using c_unique_ptr = std::unique_ptr<T, void (*)(T*)>;
}