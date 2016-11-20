QT += core

TARGET = tests_core
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++14

TEMPLATE = app
INCLUDEPATH += src
INCLUDEPATH += build/src/


include("../src/FlappyEngine.pri")


QMAKE_CXXFLAGS += -std=c++1y
DEFINES += VIEW_TYPE=GL
DEFINES += QT_NO_KEYWORDS

LIBS += -lgcov
QMAKE_CXXFLAGS += -O0 -fprofile-arcs -ftest-coverage --coverage

SOURCES += main.cpp \
    src/test_TextRes.cpp \
    src/test_JsonRes.cpp \
    src/test_ResManager.cpp

SOURCES += src/./test_entity.cpp \
    src/test_sprite.cpp \
    src/test_screenmanager.cpp \
    src/test_appmanager.cpp \
    src/test_camera.cpp \
    src/test_scenemanager.cpp \
    src/test_presenter.cpp \
    src/test_inputmanager.cpp \
    src/test_scene.cpp \
    src/test_view.cpp \
    src/test_color.cpp \
    src/test_component.cpp \
    src/test_eventsystem.cpp \
    src/test_tools.cpp \
    src/test_manager.cpp \
    src/test_classid.cpp \
    src/test_managerlist.cpp \
    src/test_transform.cpp \
    src/test_entitymanager.cpp \
    src/test_viewmanager.cpp

HEADERS += src/./testscene.h \
    src/catch.h \
    src/testcomponent.h \
    src/testview.h \
    src/fakeit.h \
    src/testmanager.h
