#pragma once

#include <cstdint>

namespace cg {
class Shape;

class ShaderGroup {
public:
    virtual ~ShaderGroup() = default;

    void setShape(const Shape& shape) { shape_ = &shape; }
    const Shape& shape() const { return *shape_; }

protected:
    ShaderGroup() : shape_(nullptr) {}

private:
    const Shape* shape_;
};
} // namespace cg
