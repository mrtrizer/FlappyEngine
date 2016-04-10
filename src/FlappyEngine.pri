INCLUDEPATH  += $$PWD

HEADERS += \
    $$PWD/shapes/gobjrect.h \
    $$PWD/shapes/gobjcircle.h \
    $$PWD/core/gobj.h \
    $$PWD/core/gobjcamera.h \
    $$PWD/core/gcollider.h \
    $$PWD/core/gworldmodel.h \
    $$PWD/core/gview.h \
    $$PWD/core/gpos.h \
    $$PWD/core/gobjpointer.h \
    $$PWD/core/gcontext.h \
    $$PWD/core/gtools.h \
    $$PWD/core/gviewfactory.h \
    $$PWD/core/gpresenter.h \
    $$PWD/core/gworldview.h \
    $$PWD/core/ganimation.h \
    $$PWD/core/gworldctrl.h \
    $$PWD/gl/gltools.h \
    $$PWD/gl/glshaderprogram.h \
    $$PWD/gl/glattribarray.h \
    $$PWD/gl/gltexture.h \
    $$PWD/gl/glviewfactory.h \
    $$PWD/gl/glworldview.h \
    $$PWD/gl/glviewshape.h \
    $$PWD/gl/glviewsprite.h \
    $$PWD/gl/glview.h \
    $$PWD/glut/glutmgr.h \
    $$PWD/qt/viewfactoryqt.h


SOURCES += \
    $$PWD/core/gobj.cpp \
    $$PWD/core/gobjcamera.cpp \
    $$PWD/core/gcollider.cpp \
    $$PWD/core/gworldmodel.cpp \
    $$PWD/core/gpos.cpp \
    $$PWD/core/gtools.cpp \
    $$PWD/core/gpresenter.cpp \
    $$PWD/core/gworldview.cpp \
    $$PWD/core/ganimation.cpp \
    $$PWD/core/gview.cpp \
    $$PWD/core/gworldctrl.cpp \
    $$PWD/gl/gltools.cpp \
    $$PWD/gl/glshaderprogram.cpp \
    $$PWD/gl/glattribarray.cpp \
    $$PWD/gl/gltexture.cpp \
    $$PWD/gl/glviewfactory.cpp \
    $$PWD/gl/glworldview.cpp \
    $$PWD/gl/glviewshape.cpp \
    $$PWD/gl/glviewsprite.cpp \
    $$PWD/glut/glutmgr.cpp \
    $$PWD/qt/viewfactoryqt.cpp

LIBS += -lGL -lglut -lGLEW -lGLU
