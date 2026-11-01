#pragma once

#include "Vector3.hpp"

class Line3 {
public:
    Vector3 a;
    Vector3 v;
    double qa;

    Line3(const Vector3& point_a, const Vector3& point_b);

    Vector3 calc_point(double lambda) const;
    double dist_point(const Vector3& point) const;
};
