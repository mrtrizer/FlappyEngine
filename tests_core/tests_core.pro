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

SOURCES += main.cpp

SOURCES += src/./test_entity.cpp
SOURCES += src/./test_sprite.cpp
SOURCES += src/./test_screenmanager.cpp
SOURCES += src/./test_atlas.cpp
SOURCES += src/./test_appmanager.cpp
SOURCES += src/./test_camera.cpp
SOURCES += src/./test_scenemanager.cpp
SOURCES += src/./test_quad.cpp
SOURCES += src/./test_presenter.cpp
SOURCES += src/./test_inputmanager.cpp
SOURCES += src/./test_scene.cpp
SOURCES += src/./test_view.cpp
SOURCES += src/./test_color.cpp
SOURCES += src/./test_component.cpp
SOURCES += src/./test_eventsystem.cpp
SOURCES += src/./test_tools.cpp
SOURCES += src/./test_manager.cpp
SOURCES += src/./test_classid.cpp
SOURCES += src/./test_reshandler.cpp
SOURCES += src/./test_managerlist.cpp
SOURCES += src/./test_textfile.cpp
SOURCES += src/./test_texture.cpp
SOURCES += src/./test_transform.cpp
SOURCES += src/./test_entitymanager.cpp
SOURCES += src/./test_resmanager.cpp
SOURCES += src/./test_viewmanager.cpp

HEADERS += src/./testscene.h
HEADERS += src/./catch.h
HEADERS += src/./testcomponent.h
HEADERS += src/./testview.h
HEADERS += src/./fakeit.h
HEADERS += src/./testmanager.h
