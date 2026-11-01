#include <geom3/triangle3.hpp>

#include <stdexcept>

namespace {
Vector3 triangle_normal(const Vector3& ab, const Vector3& ac) {
    const Vector3 cross = ab.Cross(ac);
    if (cross.is_zero()) {
        throw std::invalid_argument("A triangle must have three non-collinear vertices");
    }
    return cross.normalized();
}

bool is_inside(const Vector3& barycentric) {
    return barycentric.x >= 0.0 && barycentric.x <= 1.0
        && barycentric.y >= 0.0 && barycentric.y <= 1.0
        && barycentric.z >= 0.0 && barycentric.z <= 1.0;
}
}

Triangle3::Triangle3(const Vector3& point_a, const Vector3& point_b, const Vector3& point_c)
    : a(point_a),
      b(point_b),
      c(point_c),
      plane(point_a, triangle_normal(point_b - point_a, point_c - point_a)),
      ab(point_b - point_a),
      ac(point_c - point_a),
      d00(ab.Dot(ab)),
      d01(ab.Dot(ac)),
      d11(ac.Dot(ac)),
      denom(d00 * d11 - d01 * d01) {}

Vector3 Triangle3::barycentric(const Vector3& point) const {
    const Vector3 ap = point - a;
    const double d20 = ap.Dot(ab);
    const double d21 = ap.Dot(ac);
    const double bar_b = (d11 * d20 - d01 * d21) / denom;
    const double bar_c = (d00 * d21 - d01 * d20) / denom;
    const double bar_a = 1.0 - bar_b - bar_c;
    return {bar_a, bar_b, bar_c};
}

Vector3 Triangle3::normal(const Vector3& point) const {
    (void)point;
    return plane.n;
}

std::vector<double> Triangle3::intersects(const Line3& line) const {
    auto intersections = plane.intersects(line);
    if (intersections.empty()) {
        return intersections;
    }

    const Vector3 point = line.calc_point(intersections.front());
    return is_inside(barycentric(point)) ? intersections : std::vector<double>{};
}

std::optional<double> Triangle3::closest_intersection(const Line3& line) const {
    const auto intersection = plane.closest_intersection(line);
    if (!intersection) {
        return std::nullopt;
    }

    return is_inside(barycentric(line.calc_point(*intersection)))
        ? intersection
        : std::nullopt;
}
