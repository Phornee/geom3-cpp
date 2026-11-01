#pragma once

#include <cmath>
#include <ostream>

class Vector2 {
public:
    double x;
    double y;

    static const Vector2 left;
    static const Vector2 right;
    static const Vector2 up;
    static const Vector2 down;
    static const Vector2 zero;

    constexpr Vector2(double x_value = 0.0, double y_value = 0.0)
        : x(x_value), y(y_value) {}

    constexpr double dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    constexpr double cross(const Vector2& other) const {
        return x * other.y - y * other.x;
    }

    constexpr double magnitude_squared() const {
        return dot(*this);
    }

    double magnitude() const {
        return std::sqrt(magnitude_squared());
    }

    bool is_zero() const {
        return x == 0.0 && y == 0.0;
    }

    Vector2 normalized() const {
        const double length = magnitude();
        return length == 0.0 ? zero : *this / length;
    }

    void normalize() {
        *this = normalized();
    }

    Vector2 project(const Vector2& other) const {
        const double denominator = magnitude_squared();
        return denominator == 0.0 ? zero : *this * (dot(other) / denominator);
    }

    Vector2 reflect(const Vector2& normal) const {
        const Vector2 unit_normal = normal.normalized();
        return *this - 2.0 * dot(unit_normal) * unit_normal;
    }

    double distance(const Vector2& other) const {
        return (other - *this).magnitude();
    }

    constexpr Vector2 operator*(double scalar) const {
        return {scalar * x, scalar * y};
    }

    friend constexpr Vector2 operator*(double scalar, const Vector2& vector) {
        return vector * scalar;
    }

    constexpr Vector2 operator/(double scalar) const {
        return {x / scalar, y / scalar};
    }

    constexpr Vector2 operator+(const Vector2& other) const {
        return {x + other.x, y + other.y};
    }

    constexpr Vector2 operator-(const Vector2& other) const {
        return {x - other.x, y - other.y};
    }

    constexpr bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }

    constexpr bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }

    static void set_zero(Vector2& other) {
        other = zero;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vector2& vector);
};
