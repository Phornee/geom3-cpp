#include <geom3/vector3.hpp>

const Vector3 Vector3::forward{0.0, 0.0, 1.0};
const Vector3 Vector3::backward{0.0, 0.0, -1.0};
const Vector3 Vector3::left{-1.0, 0.0, 0.0};
const Vector3 Vector3::right{1.0, 0.0, 0.0};
const Vector3 Vector3::up{0.0, 1.0, 0.0};
const Vector3 Vector3::down{0.0, -1.0, 0.0};
const Vector3 Vector3::zero{0.0, 0.0, 0.0};

std::ostream& operator<<(std::ostream& stream, const Vector3& vector) {
    return stream << '<' << vector.x << ",  " << vector.y << ", " << vector.z << '>';
}
