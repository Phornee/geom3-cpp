#pragma once

#include "base.hpp"
#include "line3.hpp"
#include "plane.hpp"

class Triangle3 : public Shape {
public:
    Vector3 a;
    Vector3 b;
    Vector3 c;
    Plane   plane;

                            Triangle3(const Vector3& point_a, const Vector3& point_b, const Vector3& point_c);

    Vector3                 barycentric(const Vector3& point) const override;
    Vector3                 normal(const Vector3& point) const override;
                            std::vector<Hit>        intersects(const Line3& line) const override;
                            std::optional<Hit>      closest_intersection(const Line3& line) const override;

private:
    Vector3 ab_;
    Vector3 ac_;
    double d00_;
    double d01_;
    double d11_;
    double denom_;
};
