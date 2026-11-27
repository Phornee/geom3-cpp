#include <geom3/triangle3.hpp>

#include <stdexcept>
#include <utility>

namespace {
Vector3 triangle_normal(const Vector3& ab, const Vector3& ac) {
    const Vector3 cross = ab.cross(ac);
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
      ab_(point_b - point_a),
      ac_(point_c - point_a),
      d00_(ab_.dot(ab_)),
      d01_(ab_.dot(ac_)),
      d11_(ac_.dot(ac_)),
      denom_(d00_ * d11_ - d01_ * d01_) {}

Vector3 Triangle3::barycentric(const Vector3& point) const {
    const Vector3 ap = point - a;
    const double d20 = ap.dot(ab_);
    const double d21 = ap.dot(ac_);
    const double bar_b = (d11_ * d20 - d01_ * d21) / denom_;
    const double bar_c = (d00_ * d21 - d01_  * d20) / denom_;
    const double bar_a = 1.0 - bar_b - bar_c;
    return {bar_a, bar_b, bar_c};
}

Vector3 Triangle3::normal(const Vector3& point) const {
    (void)point;
    return plane.n;
}

std::vector<Hit> Triangle3::intersects(const Line3& line) const {
    const std::vector<Hit> plane_hits = plane.intersects(line);
    std::vector<Hit> hits;
    hits.reserve(plane_hits.size());
    for (Hit hit : plane_hits) {
        const Vector3 point = line.calc_point(hit.lambda);
        const Vector3 weights = barycentric(point);
        if (is_inside(weights)) {
            hit.normal = normal(point);
            hit.barycentric = weights;
            hits.push_back(std::move(hit));
        }
    }
    return hits;
}

std::optional<Hit> Triangle3::closest_intersection(const Line3& line) const {
    const auto intersection = plane.closest_intersection(line);
    if (!intersection) {
        return std::nullopt;
    }

    const Vector3 point = line.calc_point(intersection->lambda);
    const Vector3 weights = barycentric(point);
    if (!is_inside(weights)) {
        return std::nullopt;
    }

    return Hit{intersection->lambda, nullptr, normal(point), weights};
}
