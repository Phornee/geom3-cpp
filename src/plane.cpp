#include <geom3/plane.hpp>

#include <cmath>
#include <ostream>
#include <stdexcept>

namespace
{
/// Builds a unit tangent by crossing the normal with a nonparallel reference axis.
Vector3 plane_tangent(const Vector3& normal)
{
    const Vector3 reference_axis =
        std::abs(normal.z) < 0.999 ? Vector3{0.0, 0.0, 1.0} : Vector3{0.0, 1.0, 0.0};
    return normal.cross(reference_axis).normalized();
}
}

Plane::Plane(const Vector3& point, const Vector3& normal)
    : a(point),
      n(normal.normalized()),
      d(0.0),
      tangent_u_(plane_tangent(n)),
      tangent_v_(n.cross(tangent_u_).normalized()) {
    if (normal.is_zero()) {
        throw std::invalid_argument("The normal of the plane cannot be zero");
    }
    d = -n.dot(a);
}

Vector3 Plane::normal(const Vector3& point) const {
    (void)point;
    return n;
}

std::optional<Vector2> Plane::texture_coordinates(const Vector3& point, double scale) const {
    const Vector3 offset = point - a;
    return Vector2{offset.dot(tangent_u_) * scale, offset.dot(tangent_v_) * scale};
}

std::vector<double> Plane::intersects(const Line3& line) const {
    const double denominator = n.dot(line.v);
    if (denominator == 0.0) {
        return {};
    }
    return {(-n.dot(line.a) - d) / denominator};
}

std::optional<double> Plane::closest_intersection(const Line3& line) const {
    const double denominator = n.dot(line.v);
    if (denominator == 0.0) {
        return std::nullopt;
    }
    return (-n.dot(line.a) - d) / denominator;
}

std::ostream& operator<<(std::ostream& stream, const Plane& plane) {
    return stream << "A (" << plane.a.x << ", " << plane.a.y << ", " << plane.a.z
                  << ") --> N (" << plane.n.x << ", " << plane.n.y << ", " << plane.n.z << ')';
}
