#include <geom3/plane.hpp>

#include <ostream>
#include <stdexcept>

Plane::Plane(const Vector3& point, const Vector3& normal)
    : a(point), n(normal.normalized()), d(0.0) {
    if (normal.is_zero()) {
        throw std::invalid_argument("The normal of the plane cannot be zero");
    }
    d = -n.dot(a);
}

Vector3 Plane::normal(const Vector3& point) const {
    (void)point;
    return n;
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
