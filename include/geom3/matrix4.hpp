#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <stdexcept>

#include "Vector3.hpp"

class Matrix4
{
public:
    using Row = std::array<double, 4>;
    using Storage = std::array<Row, 4>;

    constexpr Matrix4() = default;
    constexpr explicit Matrix4(const Storage& values)
        : values_(values)
    {
    }

    static constexpr Matrix4 identity()
    {
        return Matrix4{Storage{{
            Row{1.0, 0.0, 0.0, 0.0},
            Row{0.0, 1.0, 0.0, 0.0},
            Row{0.0, 0.0, 1.0, 0.0},
            Row{0.0, 0.0, 0.0, 1.0}
        }}};
    }

    static Matrix4 translation(const Vector3& offset)
    {
        return Matrix4{Storage{{
            Row{1.0, 0.0, 0.0, offset.x},
            Row{0.0, 1.0, 0.0, offset.y},
            Row{0.0, 0.0, 1.0, offset.z},
            Row{0.0, 0.0, 0.0, 1.0}
        }}};
    }

    static Matrix4 scaling(const Vector3& scale)
    {
        return Matrix4{Storage{{
            Row{scale.x, 0.0, 0.0, 0.0},
            Row{0.0, scale.y, 0.0, 0.0},
            Row{0.0, 0.0, scale.z, 0.0},
            Row{0.0, 0.0, 0.0, 1.0}
        }}};
    }

    static Matrix4 scaling(double uniform_scale)
    {
        return scaling(Vector3{uniform_scale, uniform_scale, uniform_scale});
    }

    static Matrix4 rotation_x(double angle_radians)
    {
        const double cosine = std::cos(angle_radians);
        const double sine = std::sin(angle_radians);
        return Matrix4{Storage{{
            Row{1.0, 0.0, 0.0, 0.0},
            Row{0.0, cosine, -sine, 0.0},
            Row{0.0, sine, cosine, 0.0},
            Row{0.0, 0.0, 0.0, 1.0}
        }}};
    }

    static Matrix4 rotation_y(double angle_radians)
    {
        const double cosine = std::cos(angle_radians);
        const double sine = std::sin(angle_radians);
        return Matrix4{Storage{{
            Row{cosine, 0.0, sine, 0.0},
            Row{0.0, 1.0, 0.0, 0.0},
            Row{-sine, 0.0, cosine, 0.0},
            Row{0.0, 0.0, 0.0, 1.0}
        }}};
    }

    static Matrix4 rotation_z(double angle_radians)
    {
        const double cosine = std::cos(angle_radians);
        const double sine = std::sin(angle_radians);
        return Matrix4{Storage{{
            Row{cosine, -sine, 0.0, 0.0},
            Row{sine, cosine, 0.0, 0.0},
            Row{0.0, 0.0, 1.0, 0.0},
            Row{0.0, 0.0, 0.0, 1.0}
        }}};
    }

    static Matrix4 rotation_axis(const Vector3& axis, double angle_radians)
    {
        const double magnitude = axis.magnitude();
        if (magnitude == 0.0) {
            throw std::invalid_argument("A rotation axis must be non-zero");
        }

        const double x = axis.x / magnitude;
        const double y = axis.y / magnitude;
        const double z = axis.z / magnitude;
        const double cosine = std::cos(angle_radians);
        const double sine = std::sin(angle_radians);
        const double one_minus_cosine = 1.0 - cosine;

        return Matrix4{Storage{{
            Row{
                cosine + x * x * one_minus_cosine,
                x * y * one_minus_cosine - z * sine,
                x * z * one_minus_cosine + y * sine,
                0.0
            },
            Row{
                y * x * one_minus_cosine + z * sine,
                cosine + y * y * one_minus_cosine,
                y * z * one_minus_cosine - x * sine,
                0.0
            },
            Row{
                z * x * one_minus_cosine - y * sine,
                z * y * one_minus_cosine + x * sine,
                cosine + z * z * one_minus_cosine,
                0.0
            },
            Row{0.0, 0.0, 0.0, 1.0}
        }}};
    }

    constexpr Row& operator[](std::size_t row)
    {
        return values_[row];
    }

    constexpr const Row& operator[](std::size_t row) const
    {
        return values_[row];
    }

    constexpr Matrix4 operator*(const Matrix4& other) const
    {
        Matrix4 result;
        for (std::size_t row = 0; row < 4; ++row) {
            for (std::size_t column = 0; column < 4; ++column) {
                for (std::size_t inner = 0; inner < 4; ++inner) {
                    result[row][column] += values_[row][inner] * other[inner][column];
                }
            }
        }
        return result;
    }

    constexpr Vector3 transform(const Vector3& vector) const
    {
        const double x =
            values_[0][0] * vector.x +
            values_[0][1] * vector.y +
            values_[0][2] * vector.z +
            values_[0][3];
        const double y =
            values_[1][0] * vector.x +
            values_[1][1] * vector.y +
            values_[1][2] * vector.z +
            values_[1][3];
        const double z =
            values_[2][0] * vector.x +
            values_[2][1] * vector.y +
            values_[2][2] * vector.z +
            values_[2][3];
        const double w =
            values_[3][0] * vector.x +
            values_[3][1] * vector.y +
            values_[3][2] * vector.z +
            values_[3][3];

        return w == 0.0 ? Vector3{x, y, z} : Vector3{x / w, y / w, z / w};
    }

    constexpr Vector3 transform_direction(const Vector3& direction) const
    {
        return {
            values_[0][0] * direction.x +
                values_[0][1] * direction.y +
                values_[0][2] * direction.z,
            values_[1][0] * direction.x +
                values_[1][1] * direction.y +
                values_[1][2] * direction.z,
            values_[2][0] * direction.x +
                values_[2][1] * direction.y +
                values_[2][2] * direction.z
        };
    }

    constexpr Vector3 operator*(const Vector3& vector) const
    {
        return transform(vector);
    }

    double scaling_factor() const
    {
        const double x_scale = std::sqrt(
            values_[0][0] * values_[0][0] +
            values_[1][0] * values_[1][0] +
            values_[2][0] * values_[2][0]);
        const double y_scale = std::sqrt(
            values_[0][1] * values_[0][1] +
            values_[1][1] * values_[1][1] +
            values_[2][1] * values_[2][1]);
        const double z_scale = std::sqrt(
            values_[0][2] * values_[0][2] +
            values_[1][2] * values_[1][2] +
            values_[2][2] * values_[2][2]);
        return std::max({x_scale, y_scale, z_scale});
    }

    constexpr bool operator==(const Matrix4& other) const
    {
        return values_ == other.values_;
    }

    constexpr bool operator!=(const Matrix4& other) const
    {
        return !(*this == other);
    }

private:
    Storage values_{};
};
