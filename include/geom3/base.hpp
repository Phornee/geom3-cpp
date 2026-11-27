#pragma once

#include "Vector3.hpp"
#include "vector2.hpp"

#include <optional>
#include <vector>

const double epsilon = 1e-6;

class Line3;
class Object;

struct Hit
{
    double                  lambda = 0.0;
    const Object*           object = nullptr;
    Vector3                 normal = Vector3::zero;
    std::optional<Vector3>  barycentric = std::nullopt;
};

class Shape {
public:
    virtual                         ~Shape() = default;

    virtual Vector3                 barycentric(const Vector3&) const { return Vector3::zero; }
    virtual std::optional<Vector2>  texture_coordinates(const Vector3&, double) const { return std::nullopt; }
    virtual Vector3                 normal(const Vector3& point) const = 0;
    virtual std::vector<Hit>        intersects(const Line3& line) const = 0;
    virtual std::optional<Hit>      closest_intersection(const Line3& line) const = 0;
};
