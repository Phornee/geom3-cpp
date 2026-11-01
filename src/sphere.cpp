#include <geom3/sphere.hpp>

#include <algorithm>
#include <cmath>
#include <optional>
#include <ostream>
#include <utility>

namespace {
std::optional<std::pair<double, double>> roots(const Sphere& sphere, const Line3& line) {
    const Vector3 offset = line.a - sphere.c;
    const double b = 2.0 * line.v.dot(offset);
    const double c = offset.magnitude_squared() - sphere.r * sphere.r;
    const double discriminant = b * b - 4.0 * line.qa * c;

    if (discriminant < 0.0) {
        return std::nullopt;
    }

    const double square_root = std::sqrt(discriminant);
    const double denominator = 2.0 * line.qa;
    return std::pair<double, double>{
        (-b + square_root) / denominator,
        (-b - square_root) / denominator
    };
}
}

Sphere::Sphere(const Vector3& center, double radius) : c(center), r(radius) {}

Vector3 Sphere::normal(const Vector3& point) const {
    return (point - c).normalized();
}

std::vector<double> Sphere::intersects(const Line3& line) const {
    const auto solutions = roots(*this, line);
    if (!solutions) {
        return {};
    }

    const auto [first, second] = *solutions;
    if (first == second) {
        return {first};
    }
    return {first, second};
}

std::optional<double> Sphere::closest_intersection(const Line3& line) const {
    const auto solutions = roots(*this, line);
    if (!solutions) {
        return std::nullopt;
    }

    const auto [first, second] = *solutions;
    const bool first_is_positive = first > 0.0;
    const bool second_is_positive = second > 0.0;
    if (first_is_positive && second_is_positive) {
        return std::min(first, second);
    }
    if (first_is_positive) {
        return first;
    }
    if (second_is_positive) {
        return second;
    }
    return std::nullopt;
}

std::ostream& operator<<(std::ostream& stream, const Sphere& sphere) {
    return stream << '(' << sphere.c.x << ", " << sphere.c.y << ", " << sphere.c.z
                  << ") Radius = " << sphere.r;
}
