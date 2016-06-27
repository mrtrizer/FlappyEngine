#ifndef GVIEWSHAPE_H
#define GVIEWSHAPE_H

#include <memory>

#include "glview.h"
#include "glshaderprogram.h"
#include "glattribarray.h"
#include "gltools.h"

/// Contains a shader for all shapes and draw implementation.
/// All derived classes have to impmelent getAttribArray().
class GLViewShape: public GLView<GLViewShape> {
public:
    struct ColorRGBA {
      GLfloat r;
      GLfloat g;
      GLfloat b;
      GLfloat a;
    };

    GLViewShape();
    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) override;
    virtual const GLAttribArray & getAttribArray() const = 0;
    void setColorRGBA(ColorRGBA colorRGBA){this->colorRGBA = colorRGBA;}

private:
    ColorRGBA colorRGBA;
};

/// Circle shape (GL_TRIANGLE_FAN, vertexCnt points around the center point)
class GViewCircle: public GLViewShape {
public:
    GViewCircle(int vertexCnt, double r);
    const GLAttribArray & getAttribArray() const override {return circle; }

private:
    int vertexCnt;
    GLAttribArray circle;

    std::vector<GLTools::Vertex> circleTriangleFan(float r, int count);
};

/// Rect shape (GL_TRIANGLE_STRIP, two triangles with a common side)
class GViewRect: public GLViewShape {
public:
    GViewRect(float width, float height);
    const GLAttribArray & getAttribArray() const override {return rect;}

private:
    GLAttribArray rect;
};

#endif // GVIEWSHAPE_H
