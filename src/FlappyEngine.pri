INCLUDEPATH  += $$PWD

HEADERS += \
    $$PWD/core/gobjcamera.h \
    $$PWD/core/gview.h \
    $$PWD/core/gcontext.h \
    $$PWD/core/gtools.h \
    $$PWD/core/gviewfactory.h \
    $$PWD/core/gpresenter.h \
    $$PWD/core/gworldview.h \
    $$PWD/core/ganimation.h \
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
    $$PWD/qt/viewfactoryqt.h \
    $$PWD/entityx/Entity.h \
    $$PWD/entityx/Event.h \
    $$PWD/entityx/System.h \
    $$PWD/entityx/config.h \
    $$PWD/entityx/entityx.h \
    $$PWD/entityx/quick.h \
    $$PWD/core/initsystem.h \
    $$PWD/core/transform.h \
    $$PWD/core/behaviour.h \
    $$PWD/core/cbehavoiur.h \
    $$PWD/core/flappyapp.h


SOURCES += \
    $$PWD/core/gobjcamera.cpp \
    $$PWD/core/gtools.cpp \
    $$PWD/core/gpresenter.cpp \
    $$PWD/core/gworldview.cpp \
    $$PWD/core/gview.cpp \
    $$PWD/gl/gltools.cpp \
    $$PWD/gl/glshaderprogram.cpp \
    $$PWD/gl/glattribarray.cpp \
    $$PWD/gl/gltexture.cpp \
    $$PWD/gl/glviewfactory.cpp \
    $$PWD/gl/glworldview.cpp \
    $$PWD/gl/glviewshape.cpp \
    $$PWD/gl/glviewsprite.cpp \
    $$PWD/glut/glutmgr.cpp \
    $$PWD/qt/viewfactoryqt.cpp \
    $$PWD/entityx/Entity.cc \
    $$PWD/entityx/Event.cc \
    $$PWD/entityx/System.cc \
    $$PWD/entityx/help/Pool.cc \
    $$PWD/core/initsystem.cpp \
    $$PWD/core/behaviour.cpp \
    $$PWD/core/cbehavoiur.cpp \
    $$PWD/core/flappyapp.cpp

LIBS += -lGL -lglut -lGLEW -lGLU
