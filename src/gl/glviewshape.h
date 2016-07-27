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

#endif // GVIEWSHAPE_H
