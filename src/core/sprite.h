#ifndef SPRITE_H
#define SPRITE_H

#include "rectshape.h"

/// @brief Holds a spritesheet params. May be one solid frame or be divided into several equal parts.
class Sprite : public RectShape {
public:
    Sprite(std::string path, float width, float height, int frameCnt = 1):
        RectShape(width,height),
        path(path),
        frameCnt(frameCnt)
    {}
    virtual ~Sprite(){}
    inline std::string getPath() const { return path; }
    inline int getFrameCnt() const { return frameCnt; }
    void setFrameN(int frameN);
    inline int getFrameN() const { return frameN; }

protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory & factory) override;
private:
    std::string path;
    /// Current frame pointer
    int frameN = 0;
    /// Summary frame count
    int frameCnt = 1;
};

#endif // SPRITE_H
