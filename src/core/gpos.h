#ifndef GPOS_H
#define GPOS_H

#include <vector>

/// GPos determines coordinates of an object.
/// Can be extended to work with matrixes.
class GPos {
public:
    typedef std::vector<float> MvMatrix;

    GPos(float x, float y, float z):
        x(x),y(y),z(z){}
    virtual const GPos & operator* (const GPos & pos);
    virtual void move(const GPos &);
    virtual MvMatrix getMvMatrix() const;
    inline void setX(float x) {this->x = x;}
    inline void setY(float y) {this->y = y;}
    inline void setZ(float z) {this->z = z;}
    inline float getX() const {return this->x;}
    inline float getY() const {return this->y;}
    inline float getZ() const {return this->z;}

private:
    float x;
    float y;
    float z;
};

#endif // GPOS_H
