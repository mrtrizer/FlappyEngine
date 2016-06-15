QT += core

TARGET = test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
INCLUDEPATH += ../../src
INCLUDEPATH += ../../build/src/
{?printList(inc_dirs,"INCLUDEPATH += ../../*\n")?}

include("../../{!engine!}/src/FlappyEngine.pri")

QMAKE_CXXFLAGS += -std=c++1y

DEFINES += VIEW_TYPE=GL
DEFINES += QT_NO_KEYWORDS

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
