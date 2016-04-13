#include "gpos.h"

const GPos & GPos::operator* (const GPos & pos) {
    x += pos.x;
    y += pos.y;
    z += pos.z;
    return *this;
}

//TODO: Remove this function. Duplicates opeartor+=.
/// Appends offset to current coords
void GPos::move(const GPos & offset) {
    x += offset.x;
    y += offset.y;
    z += offset.z;
}

/// Returns a move matrix as a vector 4x4
GPos::MvMatrix GPos::getMvMatrix() const {
    return MvMatrix({
        1.0f, 0, 0, 0,
        0, 1.0f, 0, 0,
        0, 0, 1.0f, 0,
        getX(), getY(), getZ(), 1.0f
    });
}

GPos GPos::operator+(const GPos & right) const {
    return GPos{x + right.x, y + right.y, z + right.z};
}

GPos &GPos::operator+=(const GPos &right) {
    x += right.x;
    y += right.y;
    z += right.z;
    return *this;
}

GPos GPos::operator-() {
    return GPos{-x, -y, -z};
}

GPos GPos::operator*(const float & right) {
    return GPos{x * right, y * right, z * right};
}
