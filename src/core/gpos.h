#ifndef GPOS_H
#define GPOS_H

#include <vector>

/// GPos determines coordinates of an object.
/// Can be extended to work with matrixes.
class GPos {
public:
    typedef std::vector<float> MvMatrix;

    GPos(float x, float y, float z = 0):
        x(x),y(y),z(z){}
    const GPos & operator* (const GPos & pos);
    void move(const GPos &);
    MvMatrix getMvMatrix() const;
    inline void setX(float x) {this->x = x;}
    inline void setY(float y) {this->y = y;}
    inline void setZ(float z) {this->z = z;}
    inline float getX() const {return this->x;}
    inline float getY() const {return this->y;}
    inline float getZ() const {return this->z;}
    GPos operator+(const GPos &) const;
    GPos & operator+=(const GPos & right);
    GPos operator-();
    GPos operator*(const float & right);

private:
    float x;
    float y;
    float z;
};

#endif // GPOS_H
