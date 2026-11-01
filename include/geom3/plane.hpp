#pragma once

#include "base.hpp"
#include "line3.hpp"

class Plane : public Shape {
public:
    Vector3 a;
    Vector3 n;
    double d;

    Plane(const Vector3& point, const Vector3& normal);

    Vector3 normal(const Vector3& point) const override;
    std::vector<double> intersects(const Line3& line) const override;
    std::optional<double> closest_intersection(const Line3& line) const override;

    friend std::ostream& operator<<(std::ostream& stream, const Plane& plane);
};
