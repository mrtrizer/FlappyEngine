QT += core

TARGET = {!name!}_tests
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++14

TEMPLATE = app
INCLUDEPATH += .

{?printList(moduleNames,"include(*.pri)\n")?}

QMAKE_CXXFLAGS += -std=c++1y
DEFINES += VIEW_TYPE=GL
DEFINES += QT_NO_KEYWORDS

{?IFDEF qt ?}
{? "LIBS += " + qt["libs"] if "libs" in qt else "" ?}
{? "QMAKE_CXXFLAGS += " + qt["cxxflags"] if "cxxflags" in qt else "" ?}
{?ENDIF?}

SOURCES += main.cpp
HEADERS += catch.h
HEADERS += fakeit.h

prebuildTarget.target = prebuild
prebuildTarget.depends = FORCE
prebuildTarget.commands = flappy prebuild qt.tests
QMAKE_EXTRA_TARGETS += prebuildTarget
PRE_TARGETDEPS += prebuild

LIBS += -lGL -lglut -lGLEW -lGLU

OTHER_FILES += {?projectDir + "/config.json"?}
