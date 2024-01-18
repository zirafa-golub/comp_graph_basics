#include "core/Sphere.h"

#include "common/Math.h"

namespace cg {
const Point& Sphere::center() const { return position(); }

float Sphere::radius() const { return radius_; }

std::expected<HitDesc, Error> Sphere::hit(const Ray& ray, float tMin, float tMax) const {
    glm::vec3 originMinusCenter = ray.origin() - center();

    // calulate quadratic equation parameters
    float a = glm::dot(ray.direction(), ray.direction());
    float b = 2 * glm::dot(ray.direction(), originMinusCenter);
    float c = glm::dot(originMinusCenter, originMinusCenter) - radius_ * radius_;

    QuadSolve quadSolve = solveQuadEquation(a, b, c);

    if (quadSolve.count > 0) {
        if (isInRangeIncl(quadSolve.solutions[0], tMin, tMax)) {
            return formHitDesc(ray, quadSolve.solutions[0]);
        } else if (quadSolve.count == 2 && isInRangeIncl(quadSolve.solutions[1], tMin, tMax)) {
            return formHitDesc(ray, quadSolve.solutions[1]);
        }
    }

    return std::unexpected(ErrorCode::NotFound);
};

HitDesc Sphere::formHitDesc(const Ray& ray, float tHit) const {
    Point hitPoint = ray.evaluate(tHit);
    glm::vec3 unitNormal = (hitPoint - center()) / radius_;

    return HitDesc{this, ray, tHit, unitNormal};
}
} // namespace cg
