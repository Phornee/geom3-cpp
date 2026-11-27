#pragma once

#include "base.hpp"
#include "line3.hpp"

class Plane : public Shape {
public:
    Vector3 a;
    Vector3 n;
    double d;

                            Plane(const Vector3& point, const Vector3& normal);

    std::optional<Vector2>  texture_coordinates(const Vector3& point, double scale=1.0) const override;
    Vector3                 normal(const Vector3& point) const override;
    std::vector<Hit>        intersects(const Line3& line) const override;
    std::optional<Hit>      closest_intersection(const Line3& line) const override;

    friend std::ostream&    operator<<(std::ostream& stream, const Plane& plane);

private:
    Vector3 tangent_u_;
    Vector3 tangent_v_;
};
