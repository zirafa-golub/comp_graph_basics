#pragma once

#include "core/BasicTypes.h"
#include "core/Movable.h"
#include "core/Rotatable.h"
#include "core/Scalable.h"
#include "core/TransformUtils.h"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <cmath>

namespace cg {
class Transformable : public Movable, public Rotatable, public Scalable {
public:
    Transformable() { update(); }
    virtual ~Transformable() = default;

    Transformable(const Transformable&) = default;
    Transformable(Transformable&&) = default;
    Transformable& operator=(const Transformable&) = default;
    Transformable& operator=(Transformable&&) = default;

    void update() {
        glm::mat4 rotMat = TransformUtils::xyzRotationMatrix(rotation());
        glm::mat4 inverseRotMat = glm::transpose(rotMat);

        toGlobal_ = TransformUtils::scaleMatrix(scale());
        TransformUtils::addRotationLeftToScaleMatrix(toGlobal_, rotMat);
        TransformUtils::addTranslateLeft(toGlobal_, position());

        toLocal_ = TransformUtils::translateMatrix(-position());
        toLocal_ = inverseRotMat * toLocal_;
        TransformUtils::addScaleLeft(toLocal_, 1.0f / scale());
    }
    const glm::mat4& toGlobalFrameMatrix() const { return toGlobal_; }
    const glm::mat4& toLocalFrameMatrix() const { return toLocal_; }

private:
    glm::mat4 toGlobal_ = glm::identity<glm::mat4>();
    glm::mat4 toLocal_ = glm::identity<glm::mat4>();
};
} // namespace cg
