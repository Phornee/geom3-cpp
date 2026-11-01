#pragma once

#include "base.hpp"
#include "line3.hpp"

class Sphere : public Shape {
public:
    Vector3 c;
    double r;

    Sphere(const Vector3& center, double radius);

    Vector3 normal(const Vector3& point) const override;
    std::vector<double> intersects(const Line3& line) const override;
    std::optional<double> closest_intersection(const Line3& line) const override;

    friend std::ostream& operator<<(std::ostream& stream, const Sphere& sphere);
};
