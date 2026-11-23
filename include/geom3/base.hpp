#pragma once

#include "Vector3.hpp"
#include "vector2.hpp"

#include <optional>
#include <vector>

const double epsilon = 1e-6;

class Line3;

class Shape {
public:
    virtual                         ~Shape() = default;

    virtual Vector3                 barycentric(const Vector3&) const { return Vector3{}; }
    virtual std::optional<Vector2>  texture_coordinates(const Vector3&, double) const { return std::nullopt; }
    virtual Vector3                 normal(const Vector3& point) const = 0;
    virtual std::vector<double>     intersects(const Line3& line) const = 0;
    virtual std::optional<double>   closest_intersection(const Line3& line) const = 0;
};
