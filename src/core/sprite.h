#pragma once

#include "rectshape.h"

namespace flappy {

using namespace std;

/// @brief Holds a spritesheet params. May be one solid frame or be divided into several equal parts.
class Sprite : public RectShape {
public:
    Sprite(string path, float width, float height, int frameCnt = 1):
        RectShape(width,height),
        m_path(path),
        m_frameCnt(frameCnt)
    {}
    virtual ~Sprite(){}
    inline string path() const { return m_path; }
    inline int frameCnt() const { return m_frameCnt; }
    void setFrameN(int m_frameN);
    inline int getFrameN() const { return m_frameN; }

protected:
    virtual shared_ptr<View> makeGView(const ViewFactory & factory) override;
private:
    string m_path;
    /// Current frame pointer
    int m_frameN = 0;
    /// Summary frame count
    int m_frameCnt = 1;
};

} // flappy
