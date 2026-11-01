#pragma once

#include <cmath>
#include <ostream>

class Vector3 {
public:
    double x;
    double y;
    double z;

    static const Vector3 forward;
    static const Vector3 backward;
    static const Vector3 left;
    static const Vector3 right;
    static const Vector3 up;
    static const Vector3 down;
    static const Vector3 zero;

    constexpr Vector3(double x_value = 0.0, double y_value = 0.0, double z_value = 0.0)
        : x(x_value), y(y_value), z(z_value) {}

    constexpr double Dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    constexpr Vector3 Cross(const Vector3& other) const {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }

    constexpr double magnitudeSquared() const {
        return Dot(*this);
    }

    double magnitude() const {
        return std::sqrt(magnitudeSquared());
    }

    bool is_zero() const {
        return x == 0.0 && y == 0.0 && z == 0.0;
    }

    Vector3 normalized() const {
        const double length = magnitude();
        return length == 0.0 ? zero : *this / length;
    }

    void Normalize() {
        *this = normalized();
    }

    Vector3 Project(const Vector3& other) const {
        const double denominator = magnitudeSquared();
        return denominator == 0.0 ? zero : *this * (Dot(other) / denominator);
    }

    Vector3 Reflect(const Vector3& normal) const {
        const Vector3 unit_normal = normal.normalized();
        return *this - 2.0 * Dot(unit_normal) * unit_normal;
    }

    double Distance(const Vector3& other) const {
        return (other - *this).magnitude();
    }

    constexpr Vector3 operator*(double scalar) const {
        return {scalar * x, scalar * y, scalar * z};
    }

    friend constexpr Vector3 operator*(double scalar, const Vector3& vector) {
        return vector * scalar;
    }

    constexpr Vector3 operator/(double scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }

    constexpr Vector3 operator+(const Vector3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    constexpr Vector3 operator-(const Vector3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    constexpr bool operator==(const Vector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    constexpr bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }

    static void SetZero(Vector3& other) {
        other = zero;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vector3& vector);
};
