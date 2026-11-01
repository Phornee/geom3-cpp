#pragma once

#include "Vector3.hpp"

#include <optional>
#include <vector>

class Line3;

class Shape {
public:
    virtual ~Shape() = default;

    virtual Vector3 normal(const Vector3& point) const = 0;
    virtual std::vector<double> intersects(const Line3& line) const = 0;
    virtual std::optional<double> closest_intersection(const Line3& line) const = 0;
};
