#pragma once

#include "base.hpp"
#include "line3.hpp"
#include "plane.hpp"

class Triangle3 : public Shape {
public:
    Vector3 a;
    Vector3 b;
    Vector3 c;
    Plane plane;

    Triangle3(const Vector3& point_a, const Vector3& point_b, const Vector3& point_c);

    Vector3 barycentric(const Vector3& point) const;
    Vector3 normal(const Vector3& point) const override;
    std::vector<double> intersects(const Line3& line) const override;
    std::optional<double> closest_intersection(const Line3& line) const override;

private:
    Vector3 ab;
    Vector3 ac;
    double d00;
    double d01;
    double d11;
    double denom;
};
