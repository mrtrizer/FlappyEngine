QT += core

TARGET = {!name!}
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++14

TEMPLATE = app
INCLUDEPATH += ../../src
INCLUDEPATH += ../../build/src/
{?printList(inc_dirs,"INCLUDEPATH += ../../*\n")?}

include("../../{!engine!}/src/FlappyEngine.pri")
{?printList(moduleNames,"include(*.pri)\n")?}

QMAKE_CXXFLAGS += -std=c++1y
DEFINES += VIEW_TYPE=GL
DEFINES += QT_NO_KEYWORDS

{?IFDEF qt ?}
{? "LIBS += " + qt["libs"] if "libs" in qt else "" ?}
{? "QMAKE_CXXFLAGS += " + qt["cxxflags"] if "cxxflags" in qt else "" ?}
{?ENDIF?}

SOURCES += main.cpp
HEADERS += ../../build/src/config.h

{?printList(fileList(projectDir + "/src/",".cpp"),"SOURCES += ../../src/*\n",exclude)?}
{?printList(fileList(projectDir + "/src/",".h"),"HEADERS += ../../src/*\n",exclude)?}

prebuildTarget.target = prebuild
prebuildTarget.depends = FORCE
prebuildTarget.commands = flappy prebuild qt
QMAKE_EXTRA_TARGETS += prebuildTarget
PRE_TARGETDEPS += prebuild

RESOURCES += res.qrc
