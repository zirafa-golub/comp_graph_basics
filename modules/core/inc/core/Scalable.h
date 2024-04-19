#pragma once

#include "glm/vec3.hpp"

namespace cg {
class Scalable {
public:
    const glm::vec3& scale() const { return scale_; }
    void setScale(const glm::vec3& scale) { scale_ = scale; }
    void setScale(float x, float y, float z) { scale_ = {x, y, z}; }
    void scale(const glm::vec3& scale) { scale_ *= scale; }
    void scale(float x, float y, float z) { scale_ *= glm::vec3(x, y, z); }

private:
    glm::vec3 scale_ = {1, 1, 1};
};
} // namespace cg
