#include <geom3/line3.hpp>

#include <stdexcept>

Line3::Line3(const Vector3& point_a, const Vector3& point_b)
    : a(point_a), v(point_b - point_a), qa(v.magnitude_squared()) {
    if (qa == 0.0) {
        throw std::invalid_argument("The line cannot be defined by two equal points");
    }
}

Vector3 Line3::calc_point(double lambda) const {
    return a + v * lambda;
}

double Line3::dist_point(const Vector3& point) const {
    return (point - a).cross(v).magnitude() / std::sqrt(qa);
}
