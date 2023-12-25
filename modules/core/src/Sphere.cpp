#include "core/Sphere.h"

#include "common/Math.h"

namespace cg {
const Point& Sphere::center() const { return position(); }

float Sphere::radius() const { return radius_; }

std::expected<HitDesc, Error> Sphere::hit(const Ray& ray, float tStart, float tEnd) const {
    glm::vec3 originMinusCenter = ray.origin() - center();

    // calulate quadratic equation parameters
    float a = glm::dot(ray.direction(), ray.direction());
    float b = 2 * glm::dot(ray.direction(), originMinusCenter);
    float c = glm::dot(originMinusCenter, originMinusCenter) - radius_ * radius_;

    QuadSolve quadSolve = solveQuadEquation(a, b, c);

    if (quadSolve.count > 0) {
        if (isInRange(quadSolve.solutions[0], tStart, tEnd)) {
            return formHitDesc(ray, quadSolve.solutions[0]);
        } else if (quadSolve.count == 2 && isInRange(quadSolve.solutions[1], tStart, tEnd)) {
            return formHitDesc(ray, quadSolve.solutions[1]);
        }
    }

    return std::unexpected(ErrorCode::NotFound);
};

bool Sphere::isInRange(float x, float min, float max) { return x >= min && x <= max; }

HitDesc Sphere::formHitDesc(const Ray& ray, float tHit) const {
    Point hitPoint = ray.evaluate(tHit);
    glm::vec3 unitNormal = (hitPoint - center()) / radius_;

    return HitDesc{this, ray, tHit, std::move(unitNormal)};
}
} // namespace cg
