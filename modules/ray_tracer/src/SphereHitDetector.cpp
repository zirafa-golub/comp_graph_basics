#include "core/BasicTypes.h"
#include "core/Sphere.h"
#include "hit/SphereHitDetector.h"

#include "glm/geometric.hpp"

namespace cg {
void SphereHitDetector::initForFrame() {
    sphere_ = static_cast<const Sphere*>(&shaderGroup().shape());
    transposedLocalFrame_ = glm::transpose(sphere_->toLocalFrameMatrix());
}

std::optional<HitDesc> SphereHitDetector::hit(const Ray& ray, float rayMin, float rayMax) const {
    const auto& toLocalFrame = sphere_->toLocalFrameMatrix();
    glm::vec3 localizedOrigin = toLocalFrame * glm::vec4(ray.origin(), 1);
    glm::vec3 localizedDirection = glm::mat3(toLocalFrame) * ray.direction();
    Ray localizedRay(localizedOrigin, localizedDirection);

    // since we're doing hit detection in sphere's local frame, we don't need to figure in the sphere center because
    // it's alsways in frame origin (it's [0, 0, 0])
    const glm::vec3& centerToOrigin = localizedRay.origin();
    float radiusSquared = sphere_->radius() * sphere_->radius();
    float centerToOriginSquared = glm::dot(centerToOrigin, centerToOrigin);

    // calulate quadratic equation parameters
    float a = glm::dot(localizedRay.direction(), localizedRay.direction());
    float b = 2 * glm::dot(localizedRay.direction(), centerToOrigin);
    float c = centerToOriginSquared - radiusSquared;

    QuadSolve quadSolve = solveQuadEquation(a, b, c);

    if (quadSolve.count > 0) {
        bool isOriginOutside = radiusSquared < centerToOriginSquared;
        if (isInRangeIncl(quadSolve.solutions[0], rayMin, rayMax)) {
            return formHitDesc(ray, localizedRay, quadSolve.solutions[0], isOriginOutside);
        } else if (quadSolve.count == 2 && isInRangeIncl(quadSolve.solutions[1], rayMin, rayMax)) {
            return formHitDesc(ray, localizedRay, quadSolve.solutions[1], isOriginOutside);
        }
    }

    return std::nullopt;
}

HitDesc SphereHitDetector::formHitDesc(const Ray& originalRay, const Ray& localizedRay, float rayHitVal,
                                       bool isOriginOutside) const {
    Point hitPoint = localizedRay.evaluate(rayHitVal);

    glm::vec3 localizedNormal = isOriginOutside ? hitPoint : -hitPoint;
    // To transform the normal vector to global frame, we need the transposed inverse of the to-global-frame transform
    // which is transposed to-local-frame transform
    glm::vec3 unitNormal = glm::normalize(transposedLocalFrame_ * localizedNormal);

    return HitDesc{sphere_, originalRay, rayHitVal, unitNormal};
}
} // namespace cg
