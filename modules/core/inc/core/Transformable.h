#pragma once

#include "core/BasicTypes.h"
#include "core/Movable.h"
#include "core/Rotation.h"
#include "core/TransformUtils.h"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <cmath>

namespace cg {
class Transformable : public Movable {
public:
    Transformable() { update(); }
    virtual ~Transformable() = default;

    Transformable(const Transformable&) = default;
    Transformable(Transformable&&) = default;
    Transformable& operator=(const Transformable&) = default;
    Transformable& operator=(Transformable&&) = default;

    const Rotation& rotation() const { return rotation_; }
    void setRotation(const Rotation& rotation) { rotation_ = rotation; }
    void setRotation(Angle x, Angle y, Angle z) { rotation_ = Rotation(x, y, z); }
    void rotate(const Rotation delta) { rotation_ += delta; }
    void rotate(Angle x, Angle y, Angle z) { rotation_ += Rotation(x, y, z); }

    const glm::vec3& scale() const { return scale_; }
    void setScale(const glm::vec3& scale) { scale_ = scale; }
    void setScale(float x, float y, float z) { scale_ = {x, y, z}; }
    void scale(const glm::vec3& scale) { scale_ *= scale; }
    void scale(float x, float y, float z) { scale_ *= glm::vec3(x, y, z); }

    void update() {
        glm::mat4 rotMat = TransformUtils::xyzRotationMatrix(rotation_);
        glm::mat4 inverseRotMat = glm::transpose(rotMat);

        toGlobal_ = TransformUtils::scaleMatrix(scale_);
        TransformUtils::addRotationLeftToScaleMatrix(toGlobal_, rotMat);
        TransformUtils::addTranslateLeft(toGlobal_, position());

        toLocal_ = TransformUtils::translateMatrix(-position());
        toLocal_ = inverseRotMat * toLocal_;
        TransformUtils::addScaleLeft(toLocal_, 1.0f / scale_);

        transformUpdated();
    }
    const glm::mat4& toGlobalFrameMatrix() const { return toGlobal_; }
    const glm::mat4& toLocalFrameMatrix() const { return toLocal_; }

protected:
    virtual void transformUpdated(){};

private:
    Rotation rotation_ = Rotation{};
    glm::vec3 scale_ = {1, 1, 1};

    glm::mat4 toGlobal_ = glm::identity<glm::mat4>();
    glm::mat4 toLocal_ = glm::identity<glm::mat4>();
};
} // namespace cg
