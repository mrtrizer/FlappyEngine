#ifndef VECTOR2D_H
#define VECTOR2D_H

struct Vector2D {
    float x;
    float y;
    Vector2D operator+(const Vector2D &);
    Vector2D & operator+=(const Vector2D & right);
    Vector2D operator-();
    Vector2D operator*(const float & right);
};

#endif // VECTOR2D_H
