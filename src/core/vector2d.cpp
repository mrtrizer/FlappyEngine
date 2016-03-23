#include "vector2d.h"

Vector2D Vector2D::operator+(const Vector2D & right) {
    return Vector2D{x + right.x,y + right.y};
}

Vector2D &Vector2D::operator+=(const Vector2D &right) {
    x += right.x;
    y += right.y;
    return *this;
}

Vector2D Vector2D::operator-() {
    return Vector2D{-x,-y};
}

Vector2D Vector2D::operator*(const float & right) {
    return Vector2D{x * right, y * right};
}
