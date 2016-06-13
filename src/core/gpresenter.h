#ifndef GPRESENTER_H
#define GPRESENTER_H

#include <string>
#include <memory>

#include "gtools.h"

class GViewFactory;
class GView;

/// @brief Holds information about object representation need for view objects.
/// @details Contains a shared pointer to GView object, but doesn't calls any it's
/// methods except GView::externUpdate to say about state changing.
class GPresenter: public std::enable_shared_from_this<GPresenter> {
public:
    GPresenter()
    {}
    virtual ~GPresenter(){}
    std::shared_ptr<GView> getGView(const GViewFactory & factory);
    void cleanGView();
    void updateView();
protected:
    /// Implement and call appropriate method from factory to get your own instance of GView.
    virtual std::shared_ptr<GView> makeGView(const GViewFactory & factory) = 0;
private:
    std::shared_ptr<GView> gView;
};

/// Represents a circle shape.
class GPresenterCircle : public GPresenter {
public:
    GPresenterCircle(float r):
        r(r)
    {}
    virtual ~GPresenterCircle(){}
    inline float getR_() const { return r; }
protected:
    virtual std::shared_ptr<GView> makeGView(const GViewFactory & factory) override;
private:
    float r;
};

/// Represents a rectangle shape.
class GPresenterRect : public GPresenter {
public:
    GPresenterRect(float width, float height):
        width(width),
        height(height)
    {}
    virtual ~GPresenterRect(){}
    inline float getWidth() const { return width; }
    inline float getHeight() const { return height; }
protected:
    virtual std::shared_ptr<GView> makeGView(const GViewFactory & factory) override;
private:
    float width;
    float height;
};

/// @brief Holds a spritesheet params. May be one solid frame or be divided into several equal parts.
class GPresenterSprite : public GPresenterRect {
public:
    GPresenterSprite(std::string path, float width, float height, int frameCnt = 1):
        GPresenterRect(width,height),
        path(path),
        frameCnt(frameCnt)
    {}
    virtual ~GPresenterSprite(){}
    inline std::string getPath() const { return path; }
    inline int getFrameCnt() const { return frameCnt; }
    void setFrameN(int frameN);
    inline int getFrameN() const { return frameN; }

protected:
    virtual std::shared_ptr<GView> makeGView(const GViewFactory & factory) override;
private:
    std::string path;
    /// Current frame pointer
    int frameN = 0;
    /// Summary frame count
    int frameCnt = 1;
};

#endif // GPRESENTER_H
