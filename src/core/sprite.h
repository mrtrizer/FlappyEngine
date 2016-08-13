#pragma once

#include "rectshape.h"

namespace flappy {

/// @brief Holds a spritesheet params. May be one solid frame or be divided into several equal parts.
class Sprite : public RectShape {
public:
    Sprite(std::string path, float width, float height, int frameCnt = 1):
        RectShape(width,height),
        m_path(path),
        m_frameCnt(frameCnt)
    {}
    virtual ~Sprite(){}
    inline std::string path() const { return m_path; }
    inline int frameCnt() const { return m_frameCnt; }
    void setFrameN(int m_frameN);
    inline int frameN() const { return m_frameN; }

protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory & factory) override;
private:
    std::string m_path;
    /// Current frame pointer
    int m_frameN = 0;
    /// Summary frame count
    int m_frameCnt = 1;
};

} // flappy
