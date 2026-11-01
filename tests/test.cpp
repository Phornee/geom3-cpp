#include <geom3/geom3.hpp>

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace {
bool approximately_equal(double left, double right, double tolerance = 1e-9) {
    return std::abs(left - right) <= tolerance;
}

void vector2_tests() {
    const Vector2 a{3.0, 4.0};
    const Vector2 b{1.0, 2.0};

    assert(a.dot(b) == 11.0);
    assert(a.cross(b) == 2.0);
    assert(a.magnitude_squared() == 25.0);
    assert(a.magnitude() == 5.0);
    assert((a.normalized() == Vector2{0.6, 0.8}));
    assert(a.distance(b) == std::sqrt(8.0));
    assert((a.project(b) == Vector2{1.32, 1.76}));
    assert((Vector2{1.0, -1.0}.reflect(Vector2::up) == Vector2{1.0, 1.0}));
    assert(Vector2::zero.normalized() == Vector2::zero);
    assert((Vector2::left == Vector2{-1.0, 0.0}));
    assert((2.0 * Vector2{1.0, 3.0} == Vector2{2.0, 6.0}));
    assert(((Vector2{1.0, 2.0} + Vector2{3.0, 4.0}) == Vector2{4.0, 6.0}));
    assert(((Vector2{3.0, 4.0} - Vector2{1.0, 2.0}) == Vector2{2.0, 2.0}));
    assert((Vector2{2.0, 4.0} / 2.0 == Vector2{1.0, 2.0}));
    assert((Vector2{1.0, 2.0} != Vector2{2.0, 1.0}));

    Vector2 normalized{3.0, 4.0};
    normalized.normalize();
    assert((normalized == Vector2{0.6, 0.8}));

    Vector2 reset{1.0, 2.0};
    Vector2::set_zero(reset);
    assert(reset == Vector2::zero);
}

void line_tests() {
    const Vector3 a{0.0, 0.0, 0.0};
    const Vector3 b{0.0, 0.0, 10.0};

    bool rejected_degenerate_line = false;
    try {
        const Line3 invalid_line{a, a};
    } catch (const std::invalid_argument&) {
        rejected_degenerate_line = true;
    }
    assert(rejected_degenerate_line);

    const Line3 line{a, b};
    assert(line.calc_point(0.0) == a);
    assert(line.calc_point(1.0) == b);
    assert(approximately_equal(line.dist_point({0.0, 0.0, 5.0}), 0.0));
    assert(approximately_equal(line.dist_point({3.0, 7.0, 5.0}), 7.6157731058639087));
}

void sphere_tests() {
    const Vector3 origin{0.0, 0.0, 0.0};
    const Vector3 end{0.0, 0.0, 10.0};
    const Sphere sphere{origin, 2.0};

    const Line3 line_from_origin{origin, end};
    const Line3 line_from_outside{{0.0, 0.0, -10.0}, end};
    const Line3 line_from_beyond_sphere{end, {0.0, 0.0, 20.0}};
    const Line3 tangent_line{{0.0, 2.0, 10.0}, {0.0, 2.0, 0.0}};
    const Line3 outer_line{{0.0, 2.1, 10.0}, {0.0, 2.1, 0.0}};

    assert(sphere.intersects(line_from_origin).size() == 2);
    assert(approximately_equal(*sphere.closest_intersection(line_from_outside), 0.4));
    assert(approximately_equal(*sphere.closest_intersection(line_from_origin), 0.2));
    assert(sphere.intersects(tangent_line).size() == 1);
    assert(sphere.intersects(outer_line).empty());
    assert(!sphere.closest_intersection(line_from_beyond_sphere));
}

void plane_tests() {
    const Plane plane{{0.0, 0.0, 0.0}, {1.0, 1.0, 0.0}};
    const Line3 horizontal_line{{0.0, 1.0, 0.0}, {0.0, 1.0, 10.0}};
    const Line3 tilted_line{{0.0, -1.0, 0.0}, {0.0, 1.0, 10.0}};
    const Line3 in_plane_line{{0.0, 0.0, 0.0}, {-13.5e39, 13.5e39, 83810.16789}};

    assert(plane.intersects(horizontal_line).empty());
    assert(!plane.closest_intersection(horizontal_line));
    assert(approximately_equal(*plane.closest_intersection(tilted_line), 0.5));
    assert(!plane.closest_intersection(in_plane_line));
    assert(plane.intersects(tilted_line).size() == 1);
    assert(plane.intersects(in_plane_line).empty());

    bool rejected_zero_normal = false;
    try {
        const Plane invalid_plane{{0.0, 0.0, 0.0}, Vector3::zero};
    } catch (const std::invalid_argument&) {
        rejected_zero_normal = true;
    }
    assert(rejected_zero_normal);
}

void triangle_tests() {
    const Vector3 a{0.0, 0.0, 0.0};
    const Vector3 b{0.0, 0.0, 10.0};
    const Vector3 c{0.0, 10.0, 0.0};
    const Triangle3 triangle{a, b, c};

    assert((triangle.barycentric(a) == Vector3{1.0, 0.0, 0.0}));
    assert((triangle.barycentric(b) == Vector3{0.0, 1.0, 0.0}));
    assert((triangle.barycentric(c) == Vector3{0.0, 0.0, 1.0}));
    assert((triangle.barycentric({0.0, 5.0, 5.0}) == Vector3{0.0, 0.5, 0.5}));

    const Line3 ray_inside{{10.0, 5.0, 5.0}, {0.0, 5.0, 5.0}};
    const auto intersections = triangle.intersects(ray_inside);
    assert(intersections.size() == 1);
    assert(approximately_equal(*triangle.closest_intersection(ray_inside), 1.0));

    const Vector3 outside_barycentric = triangle.barycentric({0.0, 8.0, 1.0});
    assert(approximately_equal(outside_barycentric.x, 0.1));
    assert(approximately_equal(outside_barycentric.y, 0.1));
    assert(approximately_equal(outside_barycentric.z, 0.8));

    const Line3 ray_outside{{10.0, 15.0, 15.0}, {0.0, 15.0, 15.0}};
    assert(triangle.intersects(ray_outside).empty());
    assert(!triangle.closest_intersection(ray_outside));

    bool rejected_degenerate_triangle = false;
    try {
        const Triangle3 invalid_triangle{a, b, b};
    } catch (const std::invalid_argument&) {
        rejected_degenerate_triangle = true;
    }
    assert(rejected_degenerate_triangle);
}
}

int main() {
    vector2_tests();
    line_tests();
    sphere_tests();
    plane_tests();
    triangle_tests();
    std::cout << "All geom3 tests passed.\n";
}
