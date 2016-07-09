#ifndef GPRESENTER_H
#define GPRESENTER_H

#include <string>
#include <memory>
#include <core/entitymanager.h>
#include <glm/glm.hpp>

#include "tools.h"

class ViewFactory;
class View;

/// @brief Holds information about object representation need for view objects.
/// @details Contains a shared pointer to GView object, but doesn't calls any it's
/// methods except GView::externUpdate to say about state changing.
class Presenter: public BaseComponent<Presenter>, public std::enable_shared_from_this<Presenter> {
public:
    Presenter()
    {}
    virtual ~Presenter(){}
    std::shared_ptr<View> getGView(const ViewFactory & factory);
    void cleanGView();
    void updateView();
    virtual glm::vec3 getSize() {return glm::vec3();}
    virtual void update(TimeDelta) {}

protected:
    /// Implement and call appropriate method from factory to get your own instance of GView.
    virtual std::shared_ptr<View> makeGView(const ViewFactory & factory) = 0;
private:
    std::shared_ptr<View> gView;
};

/// Represents a circle shape.
class CircleShape  : public Presenter {
public:
    CircleShape(float r):
        r(r)
    {}
    virtual ~CircleShape(){}
    inline float getR_() const { return r; }
    virtual glm::vec3 getSize() {return glm::vec3(r * 2,r * 2,0);}
protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory & factory) override;
private:
    float r;
};

/// Represents a rectangle shape.
class RectShape : public Presenter {
public:
    RectShape(float width, float height):
        width(width),
        height(height)
    {}
    virtual glm::vec3 getSize() {return glm::vec3(width,height,0);}
    virtual ~RectShape(){}
    inline float getWidth() const { return width; }
    inline float getHeight() const { return height; }
protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory & factory) override;
private:
    float width;
    float height;
};

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

#endif // GPRESENTER_H
