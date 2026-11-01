#include <geom3/vector2.hpp>

const Vector2 Vector2::left{-1.0, 0.0};
const Vector2 Vector2::right{1.0, 0.0};
const Vector2 Vector2::up{0.0, 1.0};
const Vector2 Vector2::down{0.0, -1.0};
const Vector2 Vector2::zero{0.0, 0.0};

std::ostream& operator<<(std::ostream& stream, const Vector2& vector) {
    return stream << '<' << vector.x << ", " << vector.y << '>';
}
